SECTIONS
{
    . = 0xffff000000080000;     /* base address for MMU virtual memory kernel space */ 
    .text :
    {
        KEEP(*(.text.boot))
        *(.text .text.* .gnu.linkonce.t*)
    }
    . = ALIGN(8);
    PROC_DEF_BEGIN = .;
    .text.user :
    {
        build/user* (.text)
    }
    .rodata.user : { build/user* (.rodata) }
    .data.user : { build/user* (.data) }
    .bss.user : { build/user* (.bss) }
    PROC_DEF_END = .;
    
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
    . = ALIGN(4096); /* pages are 4kb aligned */
    __pages_start = .;
    .data.pages :
    { 
        . += (3 * (1 << 12));
    }
    _end = .;

   /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}
__bss_size = (__bss_end - __bss_start) >> 3;
__KERNEL_END = _end;