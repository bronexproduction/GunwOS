;
;  ioerr.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 23.08.2022.
;

BITS 16

io_fdc_reset_failed:

    mov bx, MSG_IO_FDC_RESET_FAILED
    jmp io_err_print

io_read_sector_failed:

    mov bx, MSG_IO_READ_SECTOR_FAILED
    jmp io_err_print

io_sector_count_mismatch:

    mov bx, MSG_IO_SECTOR_COUNT_MISMATCH

io_err_print:

    call print_str_16
    cli
    hlt
