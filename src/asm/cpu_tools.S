.global get_exception_level
get_exception_level:
	mrs x0, CurrentEL
	lsr x0, x0, #2           // the least significant 2 bits are 0
	ret

.global get_stack_pointer_level
get_stack_pointer_level:
	mrs x0, spsel
	ret