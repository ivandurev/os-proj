#include "drivers/timer.h"
#include "utils/printf.h"
#include "utils/cpu_tools.h"
#include "mmio/irq.h"
#include "mmio/timer.h"
#include "schedule/scheduler.h"
#include "irq/irq.h"

const uint32_t interval = 100000; // the timer interval at which interrupts are triggered

void timer_init()
{
	*TIMER_C1 = *TIMER_CLO + interval; // set the goal to be interval away from the current clock
}
void timer_clear()
{
	*TIMER_CS = TIMER_CS_M1; // clear interrupt signal
}

void timer_irq_enable()
{
	*IRQS_ENABLE_1 = IRQ_TIMER_1; // enable interrupt line
}

void timer_irq_handle()
{
	tick(); // first find next process then reset the timer and return

	timer_init();
	timer_clear();
}