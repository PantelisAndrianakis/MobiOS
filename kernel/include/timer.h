// kernel/include/timer.h
#ifndef TIMER_H
#define TIMER_H

#include "types.h"

// Initialize PIT timer.
void timer_init(uint32_t frequency);

// Get current tick count.
uint64_t timer_get_ticks(void);

// Wait for specified number of ticks.
void timer_wait(uint32_t ticks);

// Timer IRQ handler (called from IDT).
void timer_irq_handler(void);

#endif // TIMER_H
