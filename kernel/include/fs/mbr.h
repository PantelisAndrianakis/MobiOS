// kernel/include/fs/mbr.h
#ifndef MBR_H
#define MBR_H

#include "types.h"

// MBR partition entry.
struct mbr_partition
{
	uint8_t status;           // 0x80 = bootable.
	uint8_t first_chs[3];     // First sector CHS.
	uint8_t type;             // Partition type.
	uint8_t last_chs[3];      // Last sector CHS.
	uint32_t first_lba;       // First LBA sector.
	uint32_t sector_count;    // Number of sectors.
} __attribute__((packed));

// MBR structure.
struct mbr
{
	uint8_t bootstrap[446];
	struct mbr_partition partitions[4];
	uint16_t signature;  // 0xAA55.
} __attribute__((packed));

// Partition types.
#define MBR_TYPE_FAT32_CHS  0x0B
#define MBR_TYPE_FAT32_LBA  0x0C
#define MBR_TYPE_LINUX      0x83

// Read MBR from disk.
bool mbr_read(struct mbr* mbr_data);

// Find first FAT32 partition.
struct mbr_partition* mbr_find_fat32(struct mbr* mbr_data);

#endif // MBR_H
