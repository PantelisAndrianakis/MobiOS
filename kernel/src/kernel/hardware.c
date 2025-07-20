// kernel/src/kernel/hardware.c
// NOTE: This code follows Allman style - all opening braces on new lines

#include "kernel.h"

// Input a byte from a port
uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Output a byte to a port
void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Halt the CPU
void cpu_halt(void)
{
    __asm__ volatile("hlt");
}