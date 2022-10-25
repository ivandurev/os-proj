.global irq_init
irq_init:
	adr	x0, irq_handlers
	msr	vbar_el1, x0
	ret

.global irq_enable
irq_enable:
	msr daifclr, #2 
	ret

.global irq_disable
irq_disable:
	msr	daifset, #2
	ret