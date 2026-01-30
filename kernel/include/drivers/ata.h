// kernel/include/drivers/ata.h
#ifndef ATA_H
#define ATA_H

#include "types.h"

// ATA registers (Primary Bus).
#define ATA_PRIMARY_IO       0x1F0
#define ATA_PRIMARY_CONTROL  0x3F6
#define ATA_PRIMARY_IRQ      14

// Register offsets.
#define ATA_REG_DATA         0x00
#define ATA_REG_ERROR        0x01
#define ATA_REG_FEATURES     0x01
#define ATA_REG_SECCOUNT     0x02
#define ATA_REG_LBA_LOW      0x03
#define ATA_REG_LBA_MID      0x04
#define ATA_REG_LBA_HIGH     0x05
#define ATA_REG_DRIVE        0x06
#define ATA_REG_STATUS       0x07
#define ATA_REG_COMMAND      0x07

// Status flags.
#define ATA_SR_BSY           0x80  // Busy.
#define ATA_SR_DRDY          0x40  // Drive ready.
#define ATA_SR_DRQ           0x08  // Data request ready.
#define ATA_SR_ERR           0x01  // Error.

// Commands.
#define ATA_CMD_READ_PIO     0x20
#define ATA_CMD_WRITE_PIO    0x30
#define ATA_CMD_IDENTIFY     0xEC

// Timeouts.
#define ATA_TIMEOUT          100000

// Initialize ATA driver.
bool ata_init(void);

// Read sectors from disk.
bool ata_read_sectors(uint32_t lba, uint8_t sector_count, uint16_t* buffer);

// Write sectors to disk.
bool ata_write_sectors(uint32_t lba, uint8_t sector_count, uint16_t* buffer);

// Check if drive exists.
bool ata_drive_exists(void);

#endif // ATA_H
