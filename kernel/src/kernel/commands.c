// kernel/src/kernel/commands.c
#include "commands.h"
#include "console.h"
#include "string.h"
#include "fs/fat32.h"
#include "heap.h"

// Parse command and arguments.
static void parse_cmd(const char* cmdline, char* cmd, char* arg1)
{
	int i = 0;
	int j = 0;
	
	// Clear outputs.
	cmd[0] = '\0';
	arg1[0] = '\0';
	
	// Skip leading spaces.
	while (cmdline[i] == ' ')
	{
		i++;
	}
	
	// Extract command.
	j = 0;
	while (cmdline[i] && cmdline[i] != ' ' && j < 63)
	{
		cmd[j++] = cmdline[i++];
	}
	cmd[j] = '\0';
	
	// Skip spaces.
	while (cmdline[i] == ' ')
	{
		i++;
	}
	
	// Extract argument.
	j = 0;
	while (cmdline[i] && cmdline[i] != ' ' && j < 255)
	{
		arg1[j++] = cmdline[i++];
	}
	arg1[j] = '\0';
}

// DIR command - list directory.
static void cmd_dir(const char* arg)
{
	(void)arg;  // Unused for now.
	
	console_write_line(" Directory of C:\\");
	console_write_line("");
	
	struct fat32_dir_entry entries[128];
	uint32_t count = 0;
	
	if (fat32_read_dir(fat32_get_root_cluster(), entries, &count))
	{
		for (uint32_t i = 0; i < count; i++)
		{
			char name[13];
			int ni = 0;
			
			// Extract name.
			for (int j = 0; j < 8 && entries[i].name[j] != ' '; j++)
			{
				name[ni++] = entries[i].name[j];
			}
			
			// Add extension.
			if (entries[i].name[8] != ' ')
			{
				name[ni++] = '.';
				for (int j = 8; j < 11 && entries[i].name[j] != ' '; j++)
				{
					name[ni++] = entries[i].name[j];
				}
			}
			name[ni] = '\0';
			
			// Display entry.
			if (entries[i].attributes & FAT_ATTR_DIRECTORY)
			{
				console_write("   <DIR>  ");
			}
			else
			{
				char size_str[16];
				itoa(entries[i].file_size, size_str, 10);
				
				// Pad size.
				int padding = 6 - strlen(size_str);
				for (int p = 0; p < padding; p++)
				{
					console_write(" ");
				}
				console_write(size_str);
				console_write("  ");
			}
			
			console_write_line(name);
		}
		
		console_write_line("");
		char count_str[16];
		itoa(count, count_str, 10);
		console_write("     ");
		console_write(count_str);
		console_write_line(" File(s)");
	}
	else
	{
		console_write_line("   <Directory read failed>");
	}
}

// TYPE command - display file contents.
static void cmd_type(const char* filename)
{
	if (!filename[0])
	{
		console_write_line("Missing filename");
		return;
	}
	
	struct fat32_dir_entry* entry = fat32_find_file(fat32_get_root_cluster(), filename);
	if (!entry)
	{
		console_write_line("File not found");
		return;
	}
	
	if (entry->attributes & FAT_ATTR_DIRECTORY)
	{
		console_write_line("Cannot TYPE a directory");
		return;
	}
	
	// Read file content.
	uint8_t* buffer = (uint8_t*)kmalloc(entry->file_size + 1);
	if (!buffer)
	{
		console_write_line("Out of memory");
		return;
	}
	
	if (fat32_read_file(entry, buffer, entry->file_size))
	{
		// Display content.
		for (uint32_t i = 0; i < entry->file_size; i++)
		{
			char ch = buffer[i];
			if (ch == '\n')
			{
				console_write("\r\n");
			}
			else if (ch == '\r')
			{
				// Skip.
			}
			else if (ch >= 32 && ch <= 126)
			{
				console_putchar(ch);
			}
		}
		console_write("\r\n");
	}
	else
	{
		console_write_line("Failed to read file");
	}
	
	kfree(buffer);
}

// HELP command.
static void cmd_help(void)
{
	console_write_line("MobiOS Available Commands:");
	console_write_line("");
	console_write_line("  DIR              - List files in root directory");
	console_write_line("  TYPE <file>      - Display file contents");
	console_write_line("  CLS              - Clear screen");
	console_write_line("  HELP             - Show this help");
	console_write_line("");
	console_write_line("This is a REAL operating system with:");
	console_write_line("  - ATA disk driver for hard drive I/O");
	console_write_line("  - FAT32 file system support");
	console_write_line("  - Persistent storage across reboots");
	console_write_line("");
}

// CLS command.
static void cmd_cls(void)
{
	console_clear();
}

// Process shell command.
void commands_process(const char* cmdline)
{
	char cmd[64];
	char arg1[256];
	
	parse_cmd(cmdline, cmd, arg1);
	
	if (strlen(cmd) == 0)
	{
		return;
	}
	
	// Convert to uppercase for comparison.
	for (int i = 0; cmd[i]; i++)
	{
		if (cmd[i] >= 'a' && cmd[i] <= 'z')
		{
			cmd[i] = cmd[i] - 'a' + 'A';
		}
	}
	
	// Execute commands.
	if (strcmp(cmd, "DIR") == 0)
	{
		cmd_dir(arg1);
	}
	else if (strcmp(cmd, "TYPE") == 0)
	{
		cmd_type(arg1);
	}
	else if (strcmp(cmd, "HELP") == 0)
	{
		cmd_help();
	}
	else if (strcmp(cmd, "CLS") == 0)
	{
		cmd_cls();
	}
	else
	{
		console_write_line("Bad command or file name");
	}
}
