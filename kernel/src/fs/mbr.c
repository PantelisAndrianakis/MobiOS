// kernel/src/fs/mbr.c
#include "fs/mbr.h"
#include "drivers/ata.h"
#include "console.h"
#include "string.h"

// Read MBR from disk.
bool mbr_read(struct mbr* mbr_data)
{
	// Read sector 0 (MBR).
	uint16_t buffer[256];
	if (!ata_read_sectors(0, 1, buffer))
	{
		console_write_line("MBR: Failed to read sector 0");
		return false;
	}
	
	// Copy to MBR structure.
	memcpy(mbr_data, buffer, sizeof(struct mbr));
	
	// Verify signature.
	if (mbr_data->signature != 0xAA55)
	{
		console_write_line("MBR: Invalid signature");
		return false;
	}
	
	console_write_line("MBR: Read successfully");
	return true;
}

// Find first FAT32 partition.
struct mbr_partition* mbr_find_fat32(struct mbr* mbr_data)
{
	for (int i = 0; i < 4; i++)
	{
		if (mbr_data->partitions[i].type == MBR_TYPE_FAT32_CHS ||
		    mbr_data->partitions[i].type == MBR_TYPE_FAT32_LBA)
		{
			console_write("MBR: Found FAT32 partition at index ");
			char buf[8];
			itoa(i, buf, 10);
			console_write_line(buf);
			return &mbr_data->partitions[i];
		}
	}
	
	console_write_line("MBR: No FAT32 partition found");
	return NULL;
}
