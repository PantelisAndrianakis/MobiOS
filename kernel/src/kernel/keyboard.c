// kernel/src/kernel/keyboard.c
// NOTE: This code follows Allman style - all opening braces on new lines

#include "keyboard.h"
#include "kernel.h"

// Track keyboard state
static bool shift_pressed = false;
static bool expecting_extended_key = false;

// Simple US keyboard layout mapping (scancode set 1)
// Lowercase letters and basic symbols (when shift is not pressed)
static const char scancode_ascii_map[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Uppercase letters and symbols (when shift is pressed)
static const char scancode_shift_ascii_map[128] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0, 0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0, 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Initialize keyboard
void keyboard_init(void)
{
    // Basic keyboard initialization
    // We're assuming the PS/2 controller is already initialized by the BIOS
    
    // Initialize keyboard state
    shift_pressed = false;
    expecting_extended_key = false;
}

/*
// Wait for keyboard controller to be ready for commands
static void keyboard_wait(void)
{
    // Wait until keyboard is ready (bit 1 of status is clear)
    while (inb(KBD_STATUS_PORT) & 2);
}
*/

// Check if key is available
bool keyboard_has_key(void)
{
    // Check if output buffer is full (bit 0 of status is set)
    return (inb(KBD_STATUS_PORT) & 1);
}

// Read a key from keyboard (blocking)
static uint8_t keyboard_read_scancode(void)
{
    // Wait until a key is available
    while(!keyboard_has_key());
    
    // Read the scancode
    return inb(KBD_DATA_PORT);
}

// Process a scancode and handle special keys
// Returns: ASCII character if available, 0 if not a character key
char scancode_to_ascii(uint8_t scancode)
{
    // Check if this is a key release (bit 7 set)
    if (scancode & 0x80)
    {
        // Key release - handle shift key releases
        uint8_t released_key = scancode & 0x7F;  // Clear the release bit
        
        if (released_key == KEY_LEFT_SHIFT || released_key == KEY_RIGHT_SHIFT)
        {
            shift_pressed = false;
        }
        
        return 0; // Ignore other key releases
    }
    
    // Check for extended key prefix (0xE0)
    if (scancode == KEY_EXTENDED)
    {
        expecting_extended_key = true;
        return 0;
    }
    
    // Handle extended keys
    if (expecting_extended_key)
    {
        expecting_extended_key = false;
        
        // Return special codes for arrow keys
        switch (scancode)
        {
            case KEY_ARROW_UP:    return 0x11;  // Custom code for up arrow (DC1)
            case KEY_ARROW_DOWN:  return 0x12;  // Custom code for down arrow (DC2)
            case KEY_ARROW_LEFT:  return 0x13;  // Custom code for left arrow (DC3)
            case KEY_ARROW_RIGHT: return 0x14;  // Custom code for right arrow (DC4)
            default:              return 0;     // Unhandled extended key
        }
    }
    
    // Track shift key state
    if (scancode == KEY_LEFT_SHIFT || scancode == KEY_RIGHT_SHIFT)
    {
        shift_pressed = true;
        return 0;
    }
    
    // Convert to ASCII using the appropriate map based on shift state
    if (shift_pressed)
    {
        return scancode_shift_ascii_map[scancode];
    }
    else
    {
        return scancode_ascii_map[scancode];
    }
}

// Get character from keyboard
char keyboard_getchar(void)
{
    char c;
    
    // Keep reading until we get a valid ASCII character
    while (1)
    {
        uint8_t scancode = keyboard_read_scancode();
        
        // First handle extended key prefix
        if (scancode == KEY_EXTENDED)
        {
            expecting_extended_key = true;
            
            // Get the next scancode (the actual extended key)
            scancode = keyboard_read_scancode();
            
            // Handle extended key directly
            switch (scancode)
            {
                case KEY_ARROW_UP:    return 0x11;  // Custom code for up arrow (DC1)
                case KEY_ARROW_DOWN:  return 0x12;  // Custom code for down arrow (DC2)
                case KEY_ARROW_LEFT:  return 0x13;  // Custom code for left arrow (DC3)
                case KEY_ARROW_RIGHT: return 0x14;  // Custom code for right arrow (DC4)
                default:              break;        // Unhandled extended key
            }
            
            // If we reached here, it wasn't a handled extended key
            expecting_extended_key = false;
            continue;
        }
        
        // Handle regular keys
        c = scancode_to_ascii(scancode);
        
        // If we have a valid character, return it
        if (c != 0)
        {
            return c;
        }
        
        // Special key handling
        if (scancode == KEY_BACKSPACE)
        {
            return '\b';
        }
        else if (scancode == KEY_ENTER)
        {
            return '\n';
        }
    }
}