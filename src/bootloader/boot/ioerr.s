;
;  ioerr.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 23.08.2022.
;

BITS 16

io_fdc_reset_failed:
io_read_sector_failed:
io_sector_count_mismatch:

    mov bx, IO_DISK_ERROR_MSG
    call print_err_16
