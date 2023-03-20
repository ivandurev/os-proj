SECTIONS
{
    . = 0xffff000000080000;     /* base address for MMU virtual memory kernel space */ 
    .text.boot :
    {
        KEEP(*(.text.boot))
    }
    . = ALIGN(4096);

    /* user programs stored in ram */
    PROC_DEF_BEGIN = .;
    .text.user :
    {
        build/user* (.text .text.* .gnu.linkonce.t*)
    }
    .rodata.user :
    {
        build/user* (.rodata .rodata.* .gnu.linkonce.r*)
    }
    .data.user :
    {
        build/user* (.data .data.* .gnu.linkonce.d*)
    }
    .bss.user :
    {
        . = ALIGN(16);
        build/user* (.bss .bss.*)
        build/user* (COMMON)
    }
    PROC_DEF_END = .;
    /* end of user code */
    
    /* Kernel code */
    .text :
    {
        *(.text .text.* .gnu.linkonce.t*)
    }
    .rodata :
    {
        *(.rodata .rodata.* .gnu.linkonce.r*)
    }
    PROVIDE(_data = .);
    .data :
    {
        *(.data .data.* .gnu.linkonce.d*)
    }
    .bss (NOLOAD) :
    {
        . = ALIGN(16);
        __bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        __bss_end = .;
    }
    /* end of kernel code */

    /* kernel pages */
    . = ALIGN(4096); /* pages are 4kb aligned */
    __pages_start = .;
    .data.pages :
    { 
        . += (3 * (1 << 12));
    }
    /* end of kernel pages */

    _end = .;
    /* allocatable memory starts here */

   /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}
__bss_size = (__bss_end - __bss_start) >> 3;
__KERNEL_END = _end;