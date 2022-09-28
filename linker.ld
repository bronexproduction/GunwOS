ENTRY("__kernel_start")

SECTIONS {
    . = 0x00010000;

    .text : {
        *(.text)
    }

    .rodata ALIGN (0x1000) : {
        *(.rodata)
    }

    .data ALIGN (0x1000) : {
        *(.data)
    }

    .bss : {
_BSS_START_ = .;
   *(.bss .bss.*)
   *(COMMON)
_BSS_END_ = .;
    }
}
