ENTRY("_test_entry")

SECTIONS {
    . = 0x00400000;

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
}
