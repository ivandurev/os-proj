#ifndef _IRQ_TIMER_H
#define _IRQ_TIMER_H

// Driver for raspberry PI timer (not the ARM timer)

// needs to be called first
void timer_init();

// interrupts caused by the timer - enable the preemptive scheduling
void timer_irq_enable();
void timer_irq_handle();

#endif // _IRQ_TIMER_H