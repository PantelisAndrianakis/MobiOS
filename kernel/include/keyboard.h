// kernel/include/keyboard.h

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h"

// PS/2 controller ports
#define KBD_DATA_PORT 0x60
#define KBD_STATUS_PORT 0x64
#define KBD_COMMAND_PORT 0x64

// Special keys
#define KEY_BACKSPACE 0x0E
#define KEY_ENTER 0x1C
#define KEY_ESCAPE 0x01
#define KEY_LEFT_SHIFT 0x2A
#define KEY_RIGHT_SHIFT 0x36

// Extended keys (prefixed with 0xE0)
#define KEY_EXTENDED 0xE0
#define KEY_ARROW_UP 0x48
#define KEY_ARROW_DOWN 0x50
#define KEY_ARROW_LEFT 0x4B
#define KEY_ARROW_RIGHT 0x4D

// Initialize keyboard
void keyboard_init(void);

// Check if key is available
bool keyboard_has_key(void);

// Get key from keyboard (blocking)
char keyboard_getchar(void);

// Scan code to ASCII conversion
char scancode_to_ascii(uint8_t scancode);

#endif // KEYBOARD_H