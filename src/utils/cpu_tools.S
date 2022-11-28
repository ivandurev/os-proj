.global get_exception_level
get_exception_level:
	mrs x0, CurrentEL
	lsr x0, x0, #2           // the least significant 2 bits are 0
	ret

.global get_stack_pointer_level
get_stack_pointer_level:
	mrs x0, spsel
	ret

.global get_pc
get_pc:
   adr x0, .
   ret

.global set_el_sp
set_el_sp:
	msr spsel, x0
	ret
