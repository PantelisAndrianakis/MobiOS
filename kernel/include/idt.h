// kernel/include/idt.h
#ifndef IDT_H
#define IDT_H

#include "types.h"

// IDT entry structure.
struct idt_entry
{
	uint16_t offset_low;    // Offset bits 0-15.
	uint16_t selector;      // Code segment selector.
	uint8_t ist;            // Interrupt Stack Table.
	uint8_t type_attr;      // Type and attributes.
	uint16_t offset_mid;    // Offset bits 16-31.
	uint32_t offset_high;   // Offset bits 32-63.
	uint32_t zero;          // Reserved.
} __attribute__((packed));

// IDT pointer structure.
struct idt_ptr
{
	uint16_t limit;
	uint64_t base;
} __attribute__((packed));

// Initialize IDT.
void idt_init(void);

// Set IDT gate.
void idt_set_gate(uint8_t num, uint64_t handler, uint16_t selector, uint8_t flags);

// Load IDT register.
void idt_load(struct idt_ptr* idtp);

#endif // IDT_H
