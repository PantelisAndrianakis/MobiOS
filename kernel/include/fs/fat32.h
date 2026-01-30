// kernel/include/fs/fat32.h
#ifndef FAT32_H
#define FAT32_H

#include "types.h"

// FAT32 boot sector structure.
struct fat32_boot_sector
{
	uint8_t jump[3];
	uint8_t oem_name[8];
	uint16_t bytes_per_sector;
	uint8_t sectors_per_cluster;
	uint16_t reserved_sectors;
	uint8_t fat_count;
	uint16_t root_entries;  // 0 for FAT32.
	uint16_t total_sectors_16;  // 0 for FAT32.
	uint8_t media_type;
	uint16_t fat_size_16;  // 0 for FAT32.
	uint16_t sectors_per_track;
	uint16_t head_count;
	uint32_t hidden_sectors;
	uint32_t total_sectors_32;
	
	// FAT32 extended fields.
	uint32_t fat_size_32;
	uint16_t flags;
	uint16_t version;
	uint32_t root_cluster;
	uint16_t fsinfo_sector;
	uint16_t backup_boot_sector;
	uint8_t reserved[12];
	uint8_t drive_number;
	uint8_t reserved1;
	uint8_t boot_signature;
	uint32_t volume_id;
	uint8_t volume_label[11];
	uint8_t fs_type[8];
} __attribute__((packed));

// FAT32 directory entry.
struct fat32_dir_entry
{
	uint8_t name[11];        // 8.3 format.
	uint8_t attributes;      // File attributes.
	uint8_t reserved;
	uint8_t creation_time_tenth;
	uint16_t creation_time;
	uint16_t creation_date;
	uint16_t access_date;
	uint16_t cluster_high;   // High 16 bits of cluster.
	uint16_t modified_time;
	uint16_t modified_date;
	uint16_t cluster_low;    // Low 16 bits of cluster.
	uint32_t file_size;
} __attribute__((packed));

// Attributes.
#define FAT_ATTR_READ_ONLY  0x01
#define FAT_ATTR_HIDDEN     0x02
#define FAT_ATTR_SYSTEM     0x04
#define FAT_ATTR_VOLUME_ID  0x08
#define FAT_ATTR_DIRECTORY  0x10
#define FAT_ATTR_ARCHIVE    0x20
#define FAT_ATTR_LONG_NAME  0x0F

// Special cluster values.
#define FAT32_EOC           0x0FFFFFFF  // End of cluster chain.
#define FAT32_BAD           0x0FFFFFF7  // Bad cluster.
#define FAT32_FREE          0x00000000  // Free cluster.

// Initialize FAT32 file system.
bool fat32_init(uint32_t partition_lba);

// Read directory entries.
bool fat32_read_dir(uint32_t cluster, struct fat32_dir_entry* entries, uint32_t* count);

// Find file in directory.
struct fat32_dir_entry* fat32_find_file(uint32_t dir_cluster, const char* name);

// Read file content.
bool fat32_read_file(struct fat32_dir_entry* entry, uint8_t* buffer, uint32_t max_size);

// Write file content.
bool fat32_write_file(struct fat32_dir_entry* entry, const uint8_t* buffer, uint32_t size);

// Create file.
bool fat32_create_file(uint32_t dir_cluster, const char* name, uint8_t attributes);

// Delete file.
bool fat32_delete_file(uint32_t dir_cluster, const char* name);

// Get root directory cluster.
uint32_t fat32_get_root_cluster(void);

// Get full cluster number from directory entry.
uint32_t fat32_get_cluster(struct fat32_dir_entry* entry);

#endif // FAT32_H
