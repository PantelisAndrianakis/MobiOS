// kernel/include/pic.h
#ifndef PIC_H
#define PIC_H

#include "types.h"

// PIC ports.
#define PIC1_COMMAND    0x20
#define PIC1_DATA       0x21
#define PIC2_COMMAND    0xA0
#define PIC2_DATA       0xA1

// PIC commands.
#define PIC_EOI         0x20

// Initialize and remap PIC.
void pic_init(void);

// Send End-of-Interrupt signal.
void pic_send_eoi(uint8_t irq);

// Mask an IRQ.
void pic_set_mask(uint8_t irq);

// Unmask an IRQ.
void pic_clear_mask(uint8_t irq);

#endif // PIC_H
