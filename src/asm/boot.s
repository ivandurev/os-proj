.section ".text.boot"

.global _start

_start:
    mrs x1, mpidr_el1
    and x1, x1, #3
    cbz x1, 2f
    // We're not on the main core, so hang in an infinite wait loop
1:  wfe
    b   1b
2:
    ldr x1, =_start // stack goes from start up (start is 0x80000)
    mov sp, x1

    ldr x1, =__bss_start
    ldr w2, =__bss_size
3:
    cbz w2, 4f
    str xzr, [x1], #8
    sub w2, w2, #1
    cbnz w2, 3b

4:
    bl kernel_main
    b  1b
