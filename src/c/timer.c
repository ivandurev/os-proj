#include "utils/printf.h"
#include "mmio/irq.h"
#include "mmio/timer.h"

const unsigned int interval = 600000;
unsigned int curr = 0;

void irq_timer_init() {
	curr = *TIMER_CLO;
	curr += interval;
	*TIMER_C1 = curr;
}

void irq_timer_enable() {
	*IRQS_ENABLE_1 = IRQ_TIMER_1;
}

void irq_timer_handle() {
	curr += interval;
	*TIMER_C1 = curr;
	*TIMER_CS = TIMER_CS_M1;
	printf("Timer interrupt handled\n\r");
}