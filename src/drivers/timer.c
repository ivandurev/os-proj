#include "utils/printf.h"
#include "mmio/irq.h"
#include "mmio/timer.h"

const unsigned int interval = 600000;

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
	timer_init();
	timer_clear();
	printf("Timer interrupt handled\n\r");
}