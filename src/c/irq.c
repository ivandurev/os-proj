#include "irq/interrupts.h"
#include "irq/timer.h"

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


void irq_handle(int interrupt, unsigned long esr, unsigned long elr) {
	printf("Received interrupt lol %d: %s, ESR: %x, ELR: %x, %x\r\n", interrupt, irq_errors[interrupt], esr, elr, (int) (*IRQ_PENDING_BASIC));
	if(interrupt != IRQ_EL1t) {
		printf("%d: %s, ESR: %x, ELR: %x\r\n", interrupt, irq_errors[interrupt], esr, elr);
		return;
	}
	unsigned int source = (int) (*IRQ_PENDING_1);
	switch(source) {
		case (IRQ_TIMER_1):
			irq_timer_handle();
			break;
		default:
			printf("Unknown IRQ source: %x, %s, ESR: %x, ELR: %x\r\n", source, irq_errors[interrupt], esr, elr);
	}
}