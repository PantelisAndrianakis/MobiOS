// kernel/src/kernel/pic.c
#include "pic.h"
#include "kernel.h"

// Initialize and remap PIC.
void pic_init(void)
{
	// Save masks.
	uint8_t mask1 = inb(PIC1_DATA);
	uint8_t mask2 = inb(PIC2_DATA);
	
	// Start initialization sequence (ICW1).
	outb(PIC1_COMMAND, 0x11);
	io_wait();
	outb(PIC2_COMMAND, 0x11);
	io_wait();
	
	// Set vector offsets (ICW2).
	outb(PIC1_DATA, 32);  // Master PIC offset to 32.
	io_wait();
	outb(PIC2_DATA, 40);  // Slave PIC offset to 40.
	io_wait();
	
	// Tell master about slave (ICW3).
	outb(PIC1_DATA, 0x04);  // Slave on IRQ2.
	io_wait();
	outb(PIC2_DATA, 0x02);  // Slave identity.
	io_wait();
	
	// Set 8086 mode (ICW4).
	outb(PIC1_DATA, 0x01);
	io_wait();
	outb(PIC2_DATA, 0x01);
	io_wait();
	
	// Restore masks.
	outb(PIC1_DATA, mask1);
	outb(PIC2_DATA, mask2);
}

// Send End-of-Interrupt signal.
void pic_send_eoi(uint8_t irq)
{
	if (irq >= 8)
	{
		outb(PIC2_COMMAND, PIC_EOI);
	}
	outb(PIC1_COMMAND, PIC_EOI);
}

// Mask an IRQ (disable).
void pic_set_mask(uint8_t irq)
{
	uint16_t port;
	uint8_t value;
	
	if (irq < 8)
	{
		port = PIC1_DATA;
	}
	else
	{
		port = PIC2_DATA;
		irq -= 8;
	}
	
	value = inb(port) | (1 << irq);
	outb(port, value);
}

// Unmask an IRQ (enable).
void pic_clear_mask(uint8_t irq)
{
	uint16_t port;
	uint8_t value;
	
	if (irq < 8)
	{
		port = PIC1_DATA;
	}
	else
	{
		port = PIC2_DATA;
		irq -= 8;
	}
	
	value = inb(port) & ~(1 << irq);
	outb(port, value);
}
