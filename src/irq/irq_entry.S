#include "irq/interrupts.h"

// note the reason this file contains mostly macros is so that it is faster and because the registers need to remain intact

// each interrupt handler needs to be aligned
.macro irq_entry label
.align 7
	b \label
.endm

// the interrupt table needs to also be aligned
// the interrupts are defined in this order in ARM
// it is also reflected in interrupts.h and irq.c
// each handler is defined below
.align	11
.global irq_handlers
irq_handlers:
	irq_entry irq_sync_el1t	
	irq_entry irq_irq_el1t	
	irq_entry irq_fiq_el1t	
	irq_entry irq_error_el1t	

	irq_entry irq_sync_el1h	
	irq_entry irq_irq_el1h	
	irq_entry irq_fiq_el1h	
	irq_entry irq_error_el1h

	irq_entry irq_sync_el0_64
	irq_entry irq_irq_el0_64
	irq_entry irq_fiq_el0_64
	irq_entry irq_error_el0_64

	irq_entry irq_sync_el0_32
	irq_entry irq_irq_el0_32
	irq_entry irq_fiq_el0_32
	irq_entry irq_error_el0_32

// macro for saving the CPU state - might include FP and SIMD registers
// saved on the El0 stack - El1 is for interrupts and kernel operations
.macro	irq_save_state
	msr spsel, #0
	sub	sp, sp, #STACK_ALLOC
	stp	x0, x1, [sp, #16 * 0]
	stp	x2, x3, [sp, #16 * 1]
	stp	x4, x5, [sp, #16 * 2]
	stp	x6, x7, [sp, #16 * 3]
	stp	x8, x9, [sp, #16 * 4]
	stp	x10, x11, [sp, #16 * 5]
	stp	x12, x13, [sp, #16 * 6]
	stp	x14, x15, [sp, #16 * 7]
	stp	x16, x17, [sp, #16 * 8]
	stp	x18, x19, [sp, #16 * 9]
	stp	x20, x21, [sp, #16 * 10]
	stp	x22, x23, [sp, #16 * 11]
	stp	x24, x25, [sp, #16 * 12]
	stp	x26, x27, [sp, #16 * 13]
	stp	x28, x29, [sp, #16 * 14]
	str	x30, [sp, #16 * 15]
	msr spsel, #1
.endm

// restoring the registers from EL0 stack - not guaranteed to be the same ones - the process might have been switched
.macro	irq_restore_state
	msr spsel, #0
 	ldp	x0, x1, [sp, #16 * 0]
	ldp	x2, x3, [sp, #16 * 1]
	ldp	x4, x5, [sp, #16 * 2]
	ldp	x6, x7, [sp, #16 * 3]
	ldp	x8, x9, [sp, #16 * 4]
	ldp	x10, x11, [sp, #16 * 5]
	ldp	x12, x13, [sp, #16 * 6]
	ldp	x14, x15, [sp, #16 * 7]
	ldp	x16, x17, [sp, #16 * 8]
	ldp	x18, x19, [sp, #16 * 9]
	ldp	x20, x21, [sp, #16 * 10]
	ldp	x22, x23, [sp, #16 * 11]
	ldp	x24, x25, [sp, #16 * 12]
	ldp	x26, x27, [sp, #16 * 13]
	ldp	x28, x29, [sp, #16 * 14]
	ldr	x30, [sp, #16 * 15] 
	add	sp, sp, #STACK_ALLOC
	msr spsel, #1
.endm

// a generic handler which calls a C function with parameters about the interrupt
.macro irq_generic_handler label
	irq_save_state
	mov		x0, #\label // which interrupt occurred
	mrs		x1, esr_el1 // exception syndrome register
	mrs 	x2, elr_el1 // exception link register
	mrs     x3, sp_el0 // stack pointer of interrupted code
	bl	 	irq_handle
	irq_restore_state
	eret
.endm

// definitions of the different handlers
// for now they are all the same and further logic is defined in the C code
// this is done for simplicity and uniformity but further optimisations might be done by having specific handlers for each
irq_sync_el1t:
	irq_generic_handler SYNC_EL1t

irq_irq_el1t:
	irq_generic_handler IRQ_EL1t

irq_fiq_el1t:
	irq_generic_handler FIQ_EL1t

irq_error_el1t:
	irq_generic_handler ERROR_EL1t

irq_sync_el1h:
	irq_generic_handler SYNC_EL1h

irq_irq_el1h:
	irq_generic_handler IRQ_EL1h

irq_fiq_el1h:
	irq_generic_handler FIQ_EL1h

irq_error_el1h:
	irq_generic_handler ERROR_EL1h

irq_sync_el0_64:
	irq_generic_handler SYNC_EL0_64

irq_irq_el0_64:
	irq_generic_handler IRQ_EL0_64

irq_fiq_el0_64:
	irq_generic_handler FIQ_EL0_64

irq_error_el0_64:
	irq_generic_handler ERROR_EL0_64

irq_sync_el0_32:
	irq_generic_handler SYNC_EL0_32

irq_irq_el0_32:
	irq_generic_handler IRQ_EL0_32

irq_fiq_el0_32:
	irq_generic_handler FIQ_EL0_32

irq_error_el0_32:
	irq_generic_handler ERROR_EL0_32