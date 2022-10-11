#include "cpu/system_registers.h"

.section ".text.boot"

.global _start

_start:
// disable all cores but the one with id 0
    mrs x1, mpidr_el1
    and x1, x1, #3
    cbz x1, cpu_setup

// stop every non-main core
halt:
    b   halt

// various CPU settings
cpu_setup:
    // System Control Register 
    ldr x0, =SCTLR_VALUE_MMU_DISABLED
    msr sctlr_el1, x0
    
    // Architectural Feature Access Control Register
    ldr x0, =CPACR_VALUE
    msr cpacr_el1, x0

    // Hypervisor Configuration Register
    ldr x0, =HCR_VALUE
    msr hcr_el2, x0

#ifdef QEMU // emulation starts in EL2

    // Saved Program Status Register - for changing the CPU state
    ldr x0, =SPSR_VALUE
    msr spsr_el2, x0

    // Exception Link Register - branch to "restore" (enable) the set above CPU state
    adr x0, c_setup
    msr elr_el2, x0

#else // real CPU starts in EL3

    // Secure Configuration Register
    ldr x0, =SCR_VALUE
    msr scr_el3, x0

    // Saved Program Status Register - for changing the CPU state
    ldr x0, =SPSR_VALUE
    msr spsr_el3, x0

    // Exception Link Register - branch to "restore" (enable) the set above CPU state
    adr x0, c_setup
    msr elr_el3, x0

#endif

    // Run in EL1
    eret 

// prepare for running a C program
c_setup:
    // set up stack
    ldr x1, =_start // stack goes from start up (start is 0x80000)
    mov sp, x1

    // set up global uninitialised variables for C (must be 0)
    ldr x1, =__bss_start
    ldr w2, =__bss_size
bss_zero:
    cbz w2, c_start
    str xzr, [x1], #8
    sub w2, w2, #1
    cbnz w2, bss_zero


// start the C code from the main function
c_start:
    bl kernel_main
    b  halt
