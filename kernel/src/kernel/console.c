// kernel/src/kernel/console.c

#include "console.h"
#include "kernel.h"

// Hardware cursor control ports
#define CURSOR_COMMAND_PORT 0x3D4
#define CURSOR_DATA_PORT 0x3D5
#define CURSOR_HIGH_BYTE 14
#define CURSOR_LOW_BYTE 15

// Console state
static uint16_t* const video_memory = (uint16_t*) VGA_MEMORY;
static uint8_t console_color;
static uint16_t console_x;
static uint16_t console_y;

// Initialize the console
void console_init(void)
{
    console_color = VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4);
    console_x = 0;
    console_y = 0;
    console_clear();
    
    // Initialize hardware cursor
    outb(CURSOR_COMMAND_PORT, CURSOR_HIGH_BYTE);
    outb(CURSOR_DATA_PORT, 0);
    outb(CURSOR_COMMAND_PORT, CURSOR_LOW_BYTE);
    outb(CURSOR_DATA_PORT, 0);
}

// Clear the screen
void console_clear(void)
{
    // Fill screen with spaces using the current color
    uint16_t blank = ' ' | (console_color << 8);
    
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
    {
        video_memory[i] = blank;
    }
    
    console_x = 0;
    console_y = 0;
    
    // Reset hardware cursor position
    outb(CURSOR_COMMAND_PORT, CURSOR_HIGH_BYTE);
    outb(CURSOR_DATA_PORT, 0);
    outb(CURSOR_COMMAND_PORT, CURSOR_LOW_BYTE);
    outb(CURSOR_DATA_PORT, 0);
}

// Helper to calculate VGA entry value
static inline uint16_t vga_entry(char c, uint8_t color)
{
    return (uint16_t)c | ((uint16_t)color << 8);
}

// Handle scrolling if needed
static void console_scroll(void)
{
    if (console_y >= VGA_HEIGHT)
    {
        // Move all lines up one line
        for (size_t y = 0; y < VGA_HEIGHT - 1; y++)
        {
            for (size_t x = 0; x < VGA_WIDTH; x++)
            {
                size_t current = y * VGA_WIDTH + x;
                size_t next = (y + 1) * VGA_WIDTH + x;
                video_memory[current] = video_memory[next];
            }
        }
        
        // Clear the last line
        size_t last_line = (VGA_HEIGHT - 1) * VGA_WIDTH;
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            video_memory[last_line + x] = vga_entry(' ', console_color);
        }
        
        console_y = VGA_HEIGHT - 1;
    }
}

// Output a single character to the console
void console_putchar(char c)
{
    // Handle special characters
    if (c == '\n')
    {
        console_x = 0;
        console_y++;
        console_scroll();
        console_set_cursor(console_x, console_y);
        return;
    }
    
    if (c == '\r')
    {
        console_x = 0;
        console_set_cursor(console_x, console_y);
        return;
    }
    
    if (c == '\b')
    {
        // Handle backspace specially - move cursor back but don't erase
        if (console_x > 0)
        {
            console_x--;
            console_set_cursor(console_x, console_y);
        }
        else if (console_y > 0)
        {
            // Move to end of previous line
            console_y--;
            console_x = VGA_WIDTH - 1;
            console_set_cursor(console_x, console_y);
        }
        return;
    }
    
    // Write the character and advance cursor
    size_t index = console_y * VGA_WIDTH + console_x;
    video_memory[index] = vga_entry(c, console_color);
    
    console_x++;
    if (console_x >= VGA_WIDTH)
    {
        console_x = 0;
        console_y++;
        console_scroll();
    }
    
    // Update hardware cursor
    console_set_cursor(console_x, console_y);
}

// Write a string to the console
void console_write(const char* data)
{
    for (size_t i = 0; data[i] != '\0'; i++)
    {
        console_putchar(data[i]);
    }
}

// Set cursor position and update hardware cursor
void console_set_cursor(uint16_t x, uint16_t y)
{
    // Ensure coordinates are within bounds
    if (x >= VGA_WIDTH)
    {
        x = VGA_WIDTH - 1;
    }
    if (y >= VGA_HEIGHT)
    {
        y = VGA_HEIGHT - 1;
    }
    
    // Update internal cursor position
    console_x = x;
    console_y = y;
    
    // Calculate the linear position
    uint16_t pos = y * VGA_WIDTH + x;
    
    // Tell the VGA controller to update the hardware cursor
    outb(CURSOR_COMMAND_PORT, CURSOR_HIGH_BYTE);
    outb(CURSOR_DATA_PORT, (pos >> 8) & 0xFF);
    outb(CURSOR_COMMAND_PORT, CURSOR_LOW_BYTE);
    outb(CURSOR_DATA_PORT, pos & 0xFF);
}

// Get current cursor position
void console_get_cursor(uint16_t* x, uint16_t* y)
{
    if (x != NULL)
    {
        *x = console_x;
    }
    if (y != NULL)
    {
        *y = console_y;
    }
}

// Write a string followed by a newline
void console_write_line(const char* data)
{
    console_write(data);
    console_putchar('\n');
}
