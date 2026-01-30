// kernel/include/shell.h

#ifndef SHELL_H
#define SHELL_H

#include "types.h"

// Constants
#define MAX_COMMAND_LENGTH 256
#define BOOT_DISK "C:"  // Default boot disk identifier

// Shell functions
void shell_init(void);
void shell_run(void);

// Display the prompt
void shell_display_prompt(void);

// Process a command
void shell_process_command(const char* command);

// Read a line from the user
void shell_read_line(char* buffer, size_t max_length);

#endif // SHELL_H