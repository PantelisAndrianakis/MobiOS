// kernel/src/fs/fat32.c
#include "fs/fat32.h"
#include "drivers/ata.h"
#include "heap.h"
#include "string.h"
#include "console.h"

static struct fat32_boot_sector boot_sector;
static uint32_t partition_lba_offset = 0;
static uint32_t fat_start_sector = 0;
static uint32_t data_start_sector = 0;
static bool fat32_initialized = false;

// Read FAT table entry.
static uint32_t fat32_read_fat_entry(uint32_t cluster)
{
	// Calculate sector and offset.
	uint32_t fat_offset = cluster * 4;
	uint32_t fat_sector = fat_start_sector + (fat_offset / 512);
	uint32_t entry_offset = fat_offset % 512;
	
	// Read sector.
	uint16_t buffer[256];
	if (!ata_read_sectors(fat_sector, 1, buffer))
	{
		return FAT32_BAD;
	}
	
	// Extract entry.
	uint32_t* fat_table = (uint32_t*)buffer;
	uint32_t value = fat_table[entry_offset / 4] & 0x0FFFFFFF;
	
	return value;
}

// Write FAT table entry.
static bool fat32_write_fat_entry(uint32_t cluster, uint32_t value) __attribute__((unused));
static bool fat32_write_fat_entry(uint32_t cluster, uint32_t value)
{
	// Calculate sector and offset.
	uint32_t fat_offset = cluster * 4;
	uint32_t fat_sector = fat_start_sector + (fat_offset / 512);
	uint32_t entry_offset = fat_offset % 512;
	
	// Read sector.
	uint16_t buffer[256];
	if (!ata_read_sectors(fat_sector, 1, buffer))
	{
		return false;
	}
	
	// Modify entry.
	uint32_t* fat_table = (uint32_t*)buffer;
	fat_table[entry_offset / 4] = (fat_table[entry_offset / 4] & 0xF0000000) | (value & 0x0FFFFFFF);
	
	// Write sector back.
	return ata_write_sectors(fat_sector, 1, buffer);
}

// Get next cluster in chain.
static uint32_t fat32_next_cluster(uint32_t current_cluster)
{
	uint32_t next = fat32_read_fat_entry(current_cluster);
	
	if (next >= 0x0FFFFFF8)
	{
		return FAT32_EOC;
	}
	
	return next;
}

// Read cluster data.
static bool fat32_read_cluster(uint32_t cluster, uint8_t* buffer)
{
	// Calculate first sector of cluster.
	uint32_t first_sector = data_start_sector + ((cluster - 2) * boot_sector.sectors_per_cluster);
	
	// Read all sectors in cluster.
	return ata_read_sectors(first_sector, boot_sector.sectors_per_cluster, (uint16_t*)buffer);
}

// Write cluster data.
static bool fat32_write_cluster(uint32_t cluster, const uint8_t* buffer)
{
	// Calculate first sector of cluster.
	uint32_t first_sector = data_start_sector + ((cluster - 2) * boot_sector.sectors_per_cluster);
	
	// Write all sectors in cluster.
	return ata_write_sectors(first_sector, boot_sector.sectors_per_cluster, (uint16_t*)buffer);
}

// Initialize FAT32 file system.
bool fat32_init(uint32_t partition_lba)
{
	if (fat32_initialized)
	{
		return true;
	}
	
	partition_lba_offset = partition_lba;
	
	// Read boot sector.
	uint16_t buffer[256];
	if (!ata_read_sectors(partition_lba, 1, buffer))
	{
		console_write_line("FAT32: Failed to read boot sector");
		return false;
	}
	
	// Copy boot sector.
	memcpy(&boot_sector, buffer, sizeof(struct fat32_boot_sector));
	
	// Verify FAT32.
	if (boot_sector.fat_size_16 != 0)
	{
		console_write_line("FAT32: Not a FAT32 partition");
		return false;
	}
	
	// Calculate important sectors.
	fat_start_sector = partition_lba + boot_sector.reserved_sectors;
	data_start_sector = fat_start_sector + (boot_sector.fat_count * boot_sector.fat_size_32);
	
	fat32_initialized = true;
	console_write_line("FAT32: Initialized successfully");
	
	return true;
}

// Get root directory cluster.
uint32_t fat32_get_root_cluster(void)
{
	return boot_sector.root_cluster;
}

// Get full cluster number from directory entry.
uint32_t fat32_get_cluster(struct fat32_dir_entry* entry)
{
	return ((uint32_t)entry->cluster_high << 16) | entry->cluster_low;
}

// Read directory entries.
bool fat32_read_dir(uint32_t cluster, struct fat32_dir_entry* entries, uint32_t* count)
{
	if (!fat32_initialized)
	{
		return false;
	}
	
	*count = 0;
	uint32_t cluster_size = boot_sector.bytes_per_sector * boot_sector.sectors_per_cluster;
	uint8_t* cluster_buffer = (uint8_t*)kmalloc(cluster_size);
	
	if (!cluster_buffer)
	{
		return false;
	}
	
	uint32_t current_cluster = cluster;
	
	while (current_cluster != FAT32_EOC && current_cluster < 0x0FFFFFF8)
	{
		// Read cluster.
		if (!fat32_read_cluster(current_cluster, cluster_buffer))
		{
			kfree(cluster_buffer);
			return false;
		}
		
		// Parse directory entries.
		struct fat32_dir_entry* dir_entries = (struct fat32_dir_entry*)cluster_buffer;
		uint32_t entries_per_cluster = cluster_size / sizeof(struct fat32_dir_entry);
		
		for (uint32_t i = 0; i < entries_per_cluster; i++)
		{
			// Stop at end marker.
			if (dir_entries[i].name[0] == 0x00)
			{
				kfree(cluster_buffer);
				return true;
			}
			
			// Skip deleted entries and long name entries.
			if (dir_entries[i].name[0] == 0xE5 || 
			    dir_entries[i].attributes == FAT_ATTR_LONG_NAME)
			{
				continue;
			}
			
			// Copy entry.
			if (*count < 128)
			{
				memcpy(&entries[*count], &dir_entries[i], sizeof(struct fat32_dir_entry));
				(*count)++;
			}
		}
		
		// Get next cluster.
		current_cluster = fat32_next_cluster(current_cluster);
	}
	
	kfree(cluster_buffer);
	return true;
}

// Compare 8.3 filename.
static bool fat32_compare_name(const uint8_t* fat_name, const char* search_name)
{
	char name[12];
	int ni = 0;
	
	// Extract name part.
	for (int i = 0; i < 8 && fat_name[i] != ' '; i++)
	{
		name[ni++] = fat_name[i];
	}
	
	// Add dot if extension exists.
	if (fat_name[8] != ' ')
	{
		name[ni++] = '.';
		for (int i = 8; i < 11 && fat_name[i] != ' '; i++)
		{
			name[ni++] = fat_name[i];
		}
	}
	
	name[ni] = '\0';
	
	return strcmp(name, search_name) == 0;
}

// Find file in directory.
struct fat32_dir_entry* fat32_find_file(uint32_t dir_cluster, const char* name)
{
	static struct fat32_dir_entry found_entry;
	struct fat32_dir_entry entries[128];
	uint32_t count = 0;
	
	if (!fat32_read_dir(dir_cluster, entries, &count))
	{
		return NULL;
	}
	
	for (uint32_t i = 0; i < count; i++)
	{
		if (fat32_compare_name(entries[i].name, name))
		{
			memcpy(&found_entry, &entries[i], sizeof(struct fat32_dir_entry));
			return &found_entry;
		}
	}
	
	return NULL;
}

// Read file content.
bool fat32_read_file(struct fat32_dir_entry* entry, uint8_t* buffer, uint32_t max_size)
{
	if (!fat32_initialized || !entry)
	{
		return false;
	}
	
	uint32_t cluster = fat32_get_cluster(entry);
	uint32_t bytes_read = 0;
	uint32_t cluster_size = boot_sector.bytes_per_sector * boot_sector.sectors_per_cluster;
	uint8_t* cluster_buffer = (uint8_t*)kmalloc(cluster_size);
	
	if (!cluster_buffer)
	{
		return false;
	}
	
	while (cluster != FAT32_EOC && cluster < 0x0FFFFFF8 && bytes_read < entry->file_size)
	{
		// Read cluster.
		if (!fat32_read_cluster(cluster, cluster_buffer))
		{
			kfree(cluster_buffer);
			return false;
		}
		
		// Copy data.
		uint32_t bytes_to_copy = cluster_size;
		if (bytes_read + bytes_to_copy > entry->file_size)
		{
			bytes_to_copy = entry->file_size - bytes_read;
		}
		if (bytes_read + bytes_to_copy > max_size)
		{
			bytes_to_copy = max_size - bytes_read;
		}
		
		memcpy(buffer + bytes_read, cluster_buffer, bytes_to_copy);
		bytes_read += bytes_to_copy;
		
		// Get next cluster.
		cluster = fat32_next_cluster(cluster);
	}
	
	kfree(cluster_buffer);
	return true;
}

// Write file content (simplified - overwrites existing file).
bool fat32_write_file(struct fat32_dir_entry* entry, const uint8_t* buffer, uint32_t size)
{
	if (!fat32_initialized || !entry)
	{
		return false;
	}
	
	uint32_t cluster = fat32_get_cluster(entry);
	uint32_t bytes_written = 0;
	uint32_t cluster_size = boot_sector.bytes_per_sector * boot_sector.sectors_per_cluster;
	uint8_t* cluster_buffer = (uint8_t*)kmalloc(cluster_size);
	
	if (!cluster_buffer)
	{
		return false;
	}
	
	while (bytes_written < size && cluster != FAT32_EOC)
	{
		// Prepare cluster data.
		memset(cluster_buffer, 0, cluster_size);
		uint32_t bytes_to_write = cluster_size;
		if (bytes_written + bytes_to_write > size)
		{
			bytes_to_write = size - bytes_written;
		}
		
		memcpy(cluster_buffer, buffer + bytes_written, bytes_to_write);
		
		// Write cluster.
		if (!fat32_write_cluster(cluster, cluster_buffer))
		{
			kfree(cluster_buffer);
			return false;
		}
		
		bytes_written += bytes_to_write;
		
		// Get/allocate next cluster if needed.
		if (bytes_written < size)
		{
			uint32_t next = fat32_next_cluster(cluster);
			if (next == FAT32_EOC)
			{
				// Would need to allocate new cluster here.
				break;
			}
			cluster = next;
		}
	}
	
	kfree(cluster_buffer);
	
	// Update file size in directory entry (caller must write back).
	entry->file_size = bytes_written;
	
	return true;
}

// Create file (simplified).
bool fat32_create_file(uint32_t dir_cluster, const char* name, uint8_t attributes)
{
	// Simplified - would need to find free directory entry and allocate cluster.
	// For now, return false.
	(void)dir_cluster;
	(void)name;
	(void)attributes;
	return false;
}

// Delete file (simplified).
bool fat32_delete_file(uint32_t dir_cluster, const char* name)
{
	// Simplified - would need to mark directory entry as deleted and free clusters.
	// For now, return false.
	(void)dir_cluster;
	(void)name;
	return false;
}
