#include "irq/irq.h"
#include "irq/interrupts.h"
#include "drivers/timer.h"
#include "drivers/mini_uart.h"

#include "mmio/irq.h"
#include "utils/printf.h"

const char *irq_errors[] = {
	"SYNC_INVALID_EL1t",
	"IRQ_INVALID_EL1t",		
	"FIQ_INVALID_EL1t",		
	"ERROR_INVALID_EL1t",		

	"SYNC_INVALID_EL1h",		
	"IRQ_INVALID_EL1h",		
	"FIQ_INVALID_EL1h",		
	"ERROR_INVALID_EL1h",		

	"SYNC_INVALID_EL0_64",		
	"IRQ_INVALID_EL0_64",		
	"FIQ_INVALID_EL0_64",		
	"ERROR_INVALID_EL0_64",	

	"SYNC_INVALID_EL0_32",		
	"IRQ_INVALID_EL0_32",		
	"FIQ_INVALID_EL0_32",		
	"ERROR_INVALID_EL0_32"	
};


void irq_handle(uint8_t interrupt, uint64_t esr, uint64_t elr) {
	if(interrupt != IRQ_EL1t && interrupt != IRQ_EL0_64) {
		printf("%d: %s, ESR: %x, ELR: %x\r\n", interrupt, irq_errors[interrupt], esr, elr);
		return;
	}
	uint32_t source = (uint32_t) (*IRQ_PENDING_1);
	while(source != 0) // handle all interrupts - maybe some occured together or weren't handled in time - will also support nesting
	{
		if(source & IRQ_TIMER_1)
			timer_irq_handle();
		else if(source & IRQ_AUX)
			uart_irq_handle();
		else
			printf("Unknown IRQ source: %x, %s, ESR: %x, ELR: %x\r\n", source, irq_errors[interrupt], esr, elr);
		source = (uint32_t) (*IRQ_PENDING_1);
	}
}