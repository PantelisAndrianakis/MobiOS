// kernel/src/drivers/ata.c
#include "drivers/ata.h"
#include "kernel.h"
#include "console.h"

static bool ata_initialized = false;

// Wait for drive to be ready.
static bool ata_wait_ready(void)
{
	uint32_t timeout = ATA_TIMEOUT;
	uint8_t status;
	
	while (timeout--)
	{
		status = inb(ATA_PRIMARY_IO + ATA_REG_STATUS);
		if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRDY))
		{
			return true;
		}
		io_wait();
	}
	
	return false;
}

// Wait for data request.
static bool ata_wait_drq(void)
{
	uint32_t timeout = ATA_TIMEOUT;
	uint8_t status;
	
	while (timeout--)
	{
		status = inb(ATA_PRIMARY_IO + ATA_REG_STATUS);
		if (status & ATA_SR_DRQ)
		{
			return true;
		}
		if (status & ATA_SR_ERR)
		{
			return false;
		}
		io_wait();
	}
	
	return false;
}

// Check if drive exists.
bool ata_drive_exists(void)
{
	uint8_t status = inb(ATA_PRIMARY_IO + ATA_REG_STATUS);
	return status != 0xFF;
}

// Initialize ATA driver.
bool ata_init(void)
{
	if (ata_initialized)
	{
		return true;
	}
	
	// Check if drive exists.
	if (!ata_drive_exists())
	{
		console_write_line("ATA: No drive detected");
		return false;
	}
	
	// Select master drive.
	outb(ATA_PRIMARY_IO + ATA_REG_DRIVE, 0xA0);
	io_wait();
	
	// Wait for drive to be ready.
	if (!ata_wait_ready())
	{
		console_write_line("ATA: Drive not ready");
		return false;
	}
	
	ata_initialized = true;
	console_write_line("ATA: Primary master drive initialized");
	
	return true;
}

// Read sectors from disk.
bool ata_read_sectors(uint32_t lba, uint8_t sector_count, uint16_t* buffer)
{
	if (!ata_initialized)
	{
		return false;
	}
	
	// Wait for drive to be ready.
	if (!ata_wait_ready())
	{
		return false;
	}
	
	// Set up LBA and sector count.
	outb(ATA_PRIMARY_IO + ATA_REG_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));
	outb(ATA_PRIMARY_IO + ATA_REG_SECCOUNT, sector_count);
	outb(ATA_PRIMARY_IO + ATA_REG_LBA_LOW, (uint8_t)lba);
	outb(ATA_PRIMARY_IO + ATA_REG_LBA_MID, (uint8_t)(lba >> 8));
	outb(ATA_PRIMARY_IO + ATA_REG_LBA_HIGH, (uint8_t)(lba >> 16));
	
	// Send read command.
	outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_READ_PIO);
	
	// Read sectors.
	for (int i = 0; i < sector_count; i++)
	{
		// Wait for data.
		if (!ata_wait_drq())
		{
			return false;
		}
		
		// Read 256 words (512 bytes).
		for (int j = 0; j < 256; j++)
		{
			buffer[i * 256 + j] = inw(ATA_PRIMARY_IO + ATA_REG_DATA);
		}
	}
	
	return true;
}

// Write sectors to disk.
bool ata_write_sectors(uint32_t lba, uint8_t sector_count, uint16_t* buffer)
{
	if (!ata_initialized)
	{
		return false;
	}
	
	// Wait for drive to be ready.
	if (!ata_wait_ready())
	{
		return false;
	}
	
	// Set up LBA and sector count.
	outb(ATA_PRIMARY_IO + ATA_REG_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));
	outb(ATA_PRIMARY_IO + ATA_REG_SECCOUNT, sector_count);
	outb(ATA_PRIMARY_IO + ATA_REG_LBA_LOW, (uint8_t)lba);
	outb(ATA_PRIMARY_IO + ATA_REG_LBA_MID, (uint8_t)(lba >> 8));
	outb(ATA_PRIMARY_IO + ATA_REG_LBA_HIGH, (uint8_t)(lba >> 16));
	
	// Send write command.
	outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);
	
	// Write sectors.
	for (int i = 0; i < sector_count; i++)
	{
		// Wait for data request.
		if (!ata_wait_drq())
		{
			return false;
		}
		
		// Write 256 words (512 bytes).
		for (int j = 0; j < 256; j++)
		{
			outw(ATA_PRIMARY_IO + ATA_REG_DATA, buffer[i * 256 + j]);
		}
	}
	
	// Flush cache.
	outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, 0xE7);
	ata_wait_ready();
	
	return true;
}
