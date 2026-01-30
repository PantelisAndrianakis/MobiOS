// kernel/src/kernel/idt.c
#include "idt.h"
#include "kernel.h"
#include "console.h"
#include "string.h"
#include "pic.h"
#include "timer.h"

// IDT entries (256 total).
static struct idt_entry idt_entries[256];

// IDT pointer.
static struct idt_ptr idtp;

// Exception messages.
static const char* exception_messages[] =
{
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",
	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

// Exception handler stubs (defined in assembly).
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

// IRQ handler stubs (defined in assembly).
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

// Set IDT gate.
void idt_set_gate(uint8_t num, uint64_t handler, uint16_t selector, uint8_t flags)
{
	idt_entries[num].offset_low = handler & 0xFFFF;
	idt_entries[num].offset_mid = (handler >> 16) & 0xFFFF;
	idt_entries[num].offset_high = (handler >> 32) & 0xFFFFFFFF;
	idt_entries[num].selector = selector;
	idt_entries[num].ist = 0;
	idt_entries[num].type_attr = flags;
	idt_entries[num].zero = 0;
}

// Initialize IDT.
void idt_init(void)
{
	// Set up IDT pointer.
	idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
	idtp.base = (uint64_t)&idt_entries;
	
	// Clear IDT.
	memset(&idt_entries, 0, sizeof(struct idt_entry) * 256);
	
	// Install exception handlers (ISRs 0-31).
	idt_set_gate(0, (uint64_t)isr0, 0x08, 0x8E);
	idt_set_gate(1, (uint64_t)isr1, 0x08, 0x8E);
	idt_set_gate(2, (uint64_t)isr2, 0x08, 0x8E);
	idt_set_gate(3, (uint64_t)isr3, 0x08, 0x8E);
	idt_set_gate(4, (uint64_t)isr4, 0x08, 0x8E);
	idt_set_gate(5, (uint64_t)isr5, 0x08, 0x8E);
	idt_set_gate(6, (uint64_t)isr6, 0x08, 0x8E);
	idt_set_gate(7, (uint64_t)isr7, 0x08, 0x8E);
	idt_set_gate(8, (uint64_t)isr8, 0x08, 0x8E);
	idt_set_gate(9, (uint64_t)isr9, 0x08, 0x8E);
	idt_set_gate(10, (uint64_t)isr10, 0x08, 0x8E);
	idt_set_gate(11, (uint64_t)isr11, 0x08, 0x8E);
	idt_set_gate(12, (uint64_t)isr12, 0x08, 0x8E);
	idt_set_gate(13, (uint64_t)isr13, 0x08, 0x8E);
	idt_set_gate(14, (uint64_t)isr14, 0x08, 0x8E);
	idt_set_gate(15, (uint64_t)isr15, 0x08, 0x8E);
	idt_set_gate(16, (uint64_t)isr16, 0x08, 0x8E);
	idt_set_gate(17, (uint64_t)isr17, 0x08, 0x8E);
	idt_set_gate(18, (uint64_t)isr18, 0x08, 0x8E);
	idt_set_gate(19, (uint64_t)isr19, 0x08, 0x8E);
	idt_set_gate(20, (uint64_t)isr20, 0x08, 0x8E);
	idt_set_gate(21, (uint64_t)isr21, 0x08, 0x8E);
	idt_set_gate(22, (uint64_t)isr22, 0x08, 0x8E);
	idt_set_gate(23, (uint64_t)isr23, 0x08, 0x8E);
	idt_set_gate(24, (uint64_t)isr24, 0x08, 0x8E);
	idt_set_gate(25, (uint64_t)isr25, 0x08, 0x8E);
	idt_set_gate(26, (uint64_t)isr26, 0x08, 0x8E);
	idt_set_gate(27, (uint64_t)isr27, 0x08, 0x8E);
	idt_set_gate(28, (uint64_t)isr28, 0x08, 0x8E);
	idt_set_gate(29, (uint64_t)isr29, 0x08, 0x8E);
	idt_set_gate(30, (uint64_t)isr30, 0x08, 0x8E);
	idt_set_gate(31, (uint64_t)isr31, 0x08, 0x8E);
	
	// Install IRQ handlers (IRQs 0-15 mapped to interrupts 32-47).
	idt_set_gate(32, (uint64_t)irq0, 0x08, 0x8E);
	idt_set_gate(33, (uint64_t)irq1, 0x08, 0x8E);
	idt_set_gate(34, (uint64_t)irq2, 0x08, 0x8E);
	idt_set_gate(35, (uint64_t)irq3, 0x08, 0x8E);
	idt_set_gate(36, (uint64_t)irq4, 0x08, 0x8E);
	idt_set_gate(37, (uint64_t)irq5, 0x08, 0x8E);
	idt_set_gate(38, (uint64_t)irq6, 0x08, 0x8E);
	idt_set_gate(39, (uint64_t)irq7, 0x08, 0x8E);
	idt_set_gate(40, (uint64_t)irq8, 0x08, 0x8E);
	idt_set_gate(41, (uint64_t)irq9, 0x08, 0x8E);
	idt_set_gate(42, (uint64_t)irq10, 0x08, 0x8E);
	idt_set_gate(43, (uint64_t)irq11, 0x08, 0x8E);
	idt_set_gate(44, (uint64_t)irq12, 0x08, 0x8E);
	idt_set_gate(45, (uint64_t)irq13, 0x08, 0x8E);
	idt_set_gate(46, (uint64_t)irq14, 0x08, 0x8E);
	idt_set_gate(47, (uint64_t)irq15, 0x08, 0x8E);
	
	// Load IDT.
	idt_load(&idtp);
}

// Common ISR handler.
void isr_handler(uint64_t int_no, uint64_t err_code)
{
	console_write("Exception: ");
	if (int_no < 32)
	{
		console_write_line(exception_messages[int_no]);
	}
	else
	{
		console_write_line("Unknown");
	}
	
	char buf[32];
	console_write("Interrupt number: ");
	itoa(int_no, buf, 10);
	console_write_line(buf);
	
	console_write("Error code: ");
	itoa(err_code, buf, 10);
	console_write_line(buf);
	
	// Halt system.
	while (1)
	{
		cpu_halt();
	}
}

// Common IRQ handler.
void irq_handler(uint64_t int_no)
{
	// Handle specific IRQs.
	if (int_no == 32)  // IRQ0 - Timer
	{
		timer_irq_handler();
	}
	
	// Send EOI to PIC.
	if (int_no >= 32 && int_no < 48)
	{
		pic_send_eoi(int_no - 32);
	}
}
