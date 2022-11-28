#include "drivers/timer.h"
#include "utils/printf.h"
#include "utils/cpu_tools.h"
#include "mmio/irq.h"
#include "mmio/timer.h"
#include "schedule/scheduler.h"
#include "irq/irq.h"

const uint32_t interval = 100000;

void timer_init()
{
	*TIMER_C1 = *TIMER_CLO + interval;
}
void timer_clear()
{
	*TIMER_CS = TIMER_CS_M1;
}

void timer_irq_enable()
{
	*IRQS_ENABLE_1 = IRQ_TIMER_1;
}

void timer_irq_handle()
{
	printf("Interrupt EL %d %d\n", get_exception_level(), get_stack_pointer_level());
	tick(); // first find next process then reset the timer and return
	
	timer_init();
	timer_clear();
}