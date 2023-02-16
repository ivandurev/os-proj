#include "cpu_values/system_registers.h"
#include "cpu_values/mmu.h"
#include "mem/mem_values.h"

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

    // NOTE: Real CPU starts in EL2, if EL3 is needed look here: https://trustedfirmware-a.readthedocs.io/en/latest/plat/rpi3.html
    // Saved Program Status Register - for changing the CPU state
    ldr x0, =SPSR_VALUE
    msr spsr_el2, x0

    // Exception Link Register - branch to "restore" (enable) the set above CPU state
    adr x0, c_setup
    msr elr_el2, x0

    // Set up EL1 stack - used by interrupts
    ldr x0, =_start
    msr sp_el1, x0

    // Drop to EL1
    eret 

// prepare for running a C program
c_setup:
    // set up stack
    ldr x1, =KERNEL_MEMORY_BASE // stack goes from start up (start is 0x80000 offset from virtual start for kernel)
    add x1, x1, #KERNEL_STACK_SIZE
    mov sp, x1 // el0 stack - temporarily

    // set up global uninitialised variables for C (must be 0)
    ldr w1, =__bss_size
    cbz w1, mmu_setup
    ldr x0, =__bss_start
    bl mzero

// set up kernel virtual memory space - starting at 0xffff000000000000
mmu_setup:
    // zero out the page area - making any unmapped addresses invalid
    ldr x0, =__pages_start
    ldr w1, =PAGES_TOTAL_SIZE
    bl mzero

    ldr x2, =PAGE_SIZE // 4KB step for going between pages
    // PGD table (level 0) is just one and has just one entry pointer to a signle PUD (level 1)
    ldr x0, =__pages_start
    add x1, x0, #PAGE_SIZE // placing the single PUD right after the PGD
    orr x1, x1, #PGD_DESCRIPTOR_VALID_TABLE // flags for valid table pointer
    str x1, [x0]  // storing the descriptor at the first entry (0) in PGD
    add x1, x1, x2

    // PUD table (level 1) is just one and has one entry pointer to a single PMD (level 2)
    add x1, x0, #PAGE_SIZE
    orr x1, x1, #PUD_DESCRIPTOR_VALID_TABLE
    str x1, [x0]
    add x1, x1, x2

    // PMD table (level 2) is just one but has all 512 entries - each pointing to a 2MB section
    // covers exactly raspi 1gb ram
    // mapped directly all of kernel region - starting with 0xffff

    mov x2, #0x200000 // 2 MB step for physical address
    ldr x3, =DEVICE_OFFSET // stop at mmio
    mov x1, xzr // current offset
    ldr x4, =PMD_DESCRIPTOR_VALID_BLOCK_NORMAL
    orr x1, x1, x4 // apply descriptor flags - they will remain constant as those bits aren't updated

    pmd_normal_setup:
        str x1, [x0], #8 // store the descriptor and update the counter
        add x1, x1, x2 // update the physical address by 2mb
        cmp x1, x3 // stop the loop if we reach the end
        blo pmd_normal_setup

    // do the same setup for the remainder of the memory - mmio
    mov x1, x3 // initialise offset from device beginning
    ldr x3, =MEMORY_END_OFFSET // end at the end of memory
    ldr x4, =PMD_DESCRIPTOR_VALID_BLOCK_DEVICE
    orr x1, x1, x4 // apply device descriptor flags

    pmd_device_setup:
        str x1, [x0], #8
        add x1, x1, x2
        cmp x1, x3
        blo pmd_device_setup

    // set up kernel PGD address
    ldr x0, =__pages_start       
    msr ttbr1_el1, x0

    // set up tcr flags
    ldr x0, =TCR_EL1_VALUE      
    msr tcr_el1, x0

    // set up mair attributes
    ldr x0, =MAIR_EL1_VALUE
    msr mair_el1, x0

    // load the address of the c function before enabling mmu
    ldr x1, =kernel_main

    // enable MMU
    ldr x0, =SCTLR_VALUE             
    msr sctlr_el1, x0

    // start C program
    br  x1
