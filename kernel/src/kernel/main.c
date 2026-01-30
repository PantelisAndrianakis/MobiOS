// kernel/src/kernel/main.c

#include "kernel.h"
#include "console.h"
#include "keyboard.h"
#include "shell.h"
#include "idt.h"
#include "pic.h"
#include "timer.h"
#include "drivers/ata.h"
#include "fs/mbr.h"
#include "fs/fat32.h"
#include "string.h"

// Entry point called from assembly.
void kernel_main(void)
{
	// Initialize console/screen.
	console_init();
	
	// Display welcome message.
	console_write_line("MobiOS");
	console_write_line("Initializing...");
	console_putchar('\n');
	
	// Initialize IDT.
	console_write("Setting up interrupts... ");
	idt_init();
	console_write_line("OK");
	
	// Initialize PIC.
	console_write("Configuring PIC... ");
	pic_init();
	console_write_line("OK");
	
	// Initialize timer.
	console_write("Initializing timer... ");
	timer_init(100);  // 100 Hz.
	console_write_line("OK");
	
	// Enable interrupts.
	console_write("Enabling interrupts... ");
	enable_interrupts();
	console_write_line("OK");
	
	// Initialize keyboard.
	console_write("Initializing keyboard... ");
	keyboard_init();
	console_write_line("OK");
	
	// Initialize ATA disk driver.
	console_write("Initializing disk driver... ");
	if (ata_init())
	{
		console_write_line("OK");
		
		// Try to read MBR and initialize FAT32.
		console_write("Reading MBR... ");
		struct mbr mbr_data;
		if (mbr_read(&mbr_data))
		{
			console_write_line("OK");
			
			console_write("Looking for FAT32 partition... ");
			struct mbr_partition* part = mbr_find_fat32(&mbr_data);
			if (part)
			{
				console_write_line("OK");
				
				console_write("Initializing FAT32... ");
				if (fat32_init(part->first_lba))
				{
					console_write_line("OK");
					console_write_line("File system ready!");
				}
				else
				{
					console_write_line("FAILED");
					console_write_line("File system not available");
				}
			}
			else
			{
				console_write_line("Not found");
				console_write_line("No FAT32 partition detected");
			}
		}
		else
		{
			console_write_line("FAILED");
			console_write_line("Could not read disk");
		}
	}
	else
	{
		console_write_line("FAILED");
		console_write_line("No disk detected");
	}
	
	console_putchar('\n');
	console_write_line("Type HELP for available commands");
	console_putchar('\n');
	
	// Initialize shell.
	shell_init();
	
	// Start the shell.
	shell_run();
	
	// Infinite loop - should never get here.
	while(1)
	{
		cpu_halt();
	}
}