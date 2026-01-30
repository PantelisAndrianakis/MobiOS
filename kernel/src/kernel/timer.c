// kernel/src/kernel/timer.c
#include "timer.h"
#include "kernel.h"
#include "pic.h"

#define PIT_CHANNEL0    0x40
#define PIT_COMMAND     0x43
#define PIT_FREQ        1193182

static volatile uint64_t timer_ticks = 0;

// Timer IRQ handler.
void timer_irq_handler(void)
{
	timer_ticks++;
}

// Initialize PIT timer.
void timer_init(uint32_t frequency)
{
	// Calculate divisor.
	uint32_t divisor = PIT_FREQ / frequency;
	
	// Send command byte.
	outb(PIT_COMMAND, 0x36);
	
	// Send divisor.
	outb(PIT_CHANNEL0, divisor & 0xFF);
	outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
	
	// Reset tick count.
	timer_ticks = 0;
	
	// Enable timer IRQ (IRQ0).
	pic_clear_mask(0);
}

// Get current tick count.
uint64_t timer_get_ticks(void)
{
	return timer_ticks;
}

// Wait for specified number of ticks.
void timer_wait(uint32_t ticks)
{
	uint64_t target = timer_ticks + ticks;
	while (timer_ticks < target)
	{
		cpu_halt();
	}
}
