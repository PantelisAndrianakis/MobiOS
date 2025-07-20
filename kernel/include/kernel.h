// kernel/include/kernel.h
#ifndef KERNEL_H
#define KERNEL_H

#include "types.h"

// Function prototypes
void kernel_main(void);

// Hardware port I/O functions
uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);

// CPU control functions
void cpu_halt(void);

// Memory functions declarations
void* memcpy(void* dest, const void* src, size_t count);
void* memset(void* dest, int val, size_t count);

#endif // KERNEL_H