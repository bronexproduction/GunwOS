ENTRY("__kernel_start")

SECTIONS {
    . = 0xFFC10000;

    .start : {
        *(.start)
    }

    .text : {
        *(.text)
    }

    .rodata ALIGN (0x1000) : {
        *(.rodata)
    }

    .data ALIGN (0x1000) : {
        *(.data)
    }

    /*
        1 MB
    */
    . = 0xFFD00000;
    
    .bss : {
_BSS_START_ = .;
   *(.bss .bss.*)
   *(COMMON)
_BSS_END_ = .;
    }
}
