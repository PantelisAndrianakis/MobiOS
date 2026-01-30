// kernel/src/kernel/hardware.c

#include "kernel.h"

// Input a byte from a port.
uint8_t inb(uint16_t port)
{
	uint8_t ret;
	__asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

// Output a byte to a port.
void outb(uint16_t port, uint8_t val)
{
	__asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Input a word from a port.
uint16_t inw(uint16_t port)
{
	uint16_t ret;
	__asm__ volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

// Output a word to a port.
void outw(uint16_t port, uint16_t val)
{
	__asm__ volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

// Wait for I/O operation to complete.
void io_wait(void)
{
	// Port 0x80 is used for POST codes, writing to it delays.
	outb(0x80, 0);
}

// Halt the CPU.
void cpu_halt(void)
{
	__asm__ volatile("hlt");
}

// Enable interrupts.
void enable_interrupts(void)
{
	__asm__ volatile("sti");
}

// Disable interrupts.
void disable_interrupts(void)
{
	__asm__ volatile("cli");
}