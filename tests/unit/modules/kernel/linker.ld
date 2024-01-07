ENTRY("test_runner")

SECTIONS {
    . = 0x00010000;

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

    . = 0x00100000;
    .bss : {
_BSS_START_ = .;
   *(.bss .bss.*)
   *(COMMON)
_BSS_END_ = .;
    }
    . = 0x00200000;
}
