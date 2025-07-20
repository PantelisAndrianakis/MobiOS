// kernel/src/kernel/shell.c
// NOTE: This code follows Allman style - all opening braces on new lines

#include "shell.h"
#include "console.h"
#include "keyboard.h"
#include "kernel.h"

// Command buffer
static char command_buffer[MAX_COMMAND_LENGTH];
static uint16_t prompt_row = 0;  // Track the current prompt's row position

// Initialize shell
void shell_init(void)
{
    // Nothing special to initialize for now
}

// Display the command prompt
void shell_display_prompt(void)
{
    uint16_t prompt_x, prompt_y;
    
    console_write(BOOT_DISK);
    console_write("\\>");
    
    // Store the current cursor position after displaying prompt
    console_get_cursor(&prompt_x, &prompt_y);
    prompt_row = prompt_y;
}

// Read a line from the user with echoing
void shell_read_line(char* buffer, size_t max_length)
{
    size_t buffer_len = 0;  // Current length of text in buffer
    size_t cursor_pos = 0;  // Current cursor position (0 to buffer_len)
    char c;
    
    while (1)
    {
        // Wait for a key
        c = keyboard_getchar();
        
        // Handle arrow keys (custom codes defined in keyboard.c)
        if (c == 0x11)  // Up arrow - not implemented yet
        {
            continue;
        }
        else if (c == 0x12)  // Down arrow - not implemented yet
        {
            continue;
        }
        else if (c == 0x13)  // Left arrow
        {
            if (cursor_pos > 0)
            {
                cursor_pos--;
                // Use the new cursor positioning function instead of backspace
                console_set_cursor(cursor_pos + 4, prompt_row);  // +4 for prompt "C:\>"
            }
            continue;  // Skip the rest of the loop
        }
        else if (c == 0x14)  // Right arrow
        {
            if (cursor_pos < buffer_len)
            {
                cursor_pos++;
                // Set cursor position directly
                console_set_cursor(cursor_pos + 4, prompt_row);  // +4 for prompt "C:\>"
            }
            continue;  // Skip the rest of the loop
        }
        
        // Handle backspace
        if (c == '\b')
        {
            if (cursor_pos > 0)
            {
                if (cursor_pos < buffer_len)
                {
                    // Delete character at cursor_pos-1 and shift everything to the left
                    size_t i;
                    
                    // Shift characters in the buffer
                    for (i = cursor_pos - 1; i < buffer_len - 1; i++)
                    {
                        buffer[i] = buffer[i + 1];
                    }
                    
                    buffer_len--;
                    cursor_pos--;
                    
                    // Redraw the entire line
                    console_set_cursor(4, prompt_row);  // Move to start of input
                    
                    // Display the updated buffer content
                    for (i = 0; i < buffer_len; i++)
                    {
                        console_putchar(buffer[i]);
                    }
                    
                    // Clear any remaining character
                    if (buffer_len < max_length - 1)
                    {
                        console_putchar(' ');
                        
                        // Move cursor back to position
                        console_set_cursor(cursor_pos + 4, prompt_row);
                    }
                }
                else
                {
                    // Cursor at end of line, simple case
                    cursor_pos--;
                    buffer_len--;
                    
                    // Erase the last character
                    console_set_cursor(cursor_pos + 4, prompt_row);
                    console_putchar(' ');
                    console_set_cursor(cursor_pos + 4, prompt_row);
                }
            }
            continue;
        }
        
        // Handle enter/return
        if (c == '\n')
        {
            console_putchar('\n');
            buffer[buffer_len] = '\0';
            return;
        }
        
        // Ignore other control characters
        if (c < ' ' && c != '\t')
        {
            continue;
        }
        
        // Don't overflow the buffer
        if (buffer_len >= max_length - 1)
        {
            continue;
        }
        
        if (cursor_pos < buffer_len)
        {
            // Insert character at cursor position
            size_t i;
            
            // Shift characters right
            for (i = buffer_len; i > cursor_pos; i--)
            {
                buffer[i] = buffer[i - 1];
            }
            
            // Insert new character
            buffer[cursor_pos] = c;
            cursor_pos++;
            buffer_len++;
            
            // Redraw the entire line
            console_set_cursor(4, prompt_row);  // Move to start of input
            
            // Display the updated buffer content
            for (i = 0; i < buffer_len; i++)
            {
                console_putchar(buffer[i]);
            }
            
            // Position cursor at insertion point
            console_set_cursor(cursor_pos + 4, prompt_row);
        }
        else
        {
            // Append character at end of buffer
            buffer[cursor_pos] = c;
            cursor_pos++;
            buffer_len++;
            
            // Print the character
            console_putchar(c);
        }
    }
}

// Process the entered command
void shell_process_command(const char* command)
{
    // For now, just echo the command if it's not empty
    if (command[0] != '\0')
    {
        console_write("Command: ");
        console_write_line(command);
    }
    
    // In a real shell, we would parse and execute commands here
    // For example:
    // if (strcmp(command, "help") == 0) { ... }
    // else if (strcmp(command, "clear") == 0) { ... }
    // etc.
}

// Main shell loop
void shell_run(void)
{
    while (1)
    {
        // Display prompt
        shell_display_prompt();
        
        // Read command
        shell_read_line(command_buffer, MAX_COMMAND_LENGTH);
        
        // Process command
        shell_process_command(command_buffer);
    }
}