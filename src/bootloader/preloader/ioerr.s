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














































.io_read_disk_error:
    mov bx, MSG_IO_DISK_ERROR_MSG
    call print_str_16
    
    mov al, ah                      ; print error code
    call print_hex_byte

    mov bx, MSG_IO_DISK_READ_SECT       ; print sector
    call print_str_16
    mov al, [io_data_sector]
    call print_hex_byte

    mov bx, MSG_IO_DISK_READ_CYLINDER   ; print cylinder
    call print_str_16
    mov ax, [io_data_cylinder]
    call print_hex_word

    mov bx, MSG_IO_DISK_READ_HEAD       ; print head
    call print_str_16
    mov al, [io_data_head]
    call print_hex_byte
    
    mov bx, MSG_IO_DISK_READ_DRIVE      ; print drive
    call print_str_16
    mov al, [io_data_drive_index]
    call print_hex_byte

    jmp $

.io_read_disk_no_drive_data_error:
    mov bx, MSG_IO_DISK_SECTOR_NO_DRIVE_DATA_ERROR_MSG
    call print_str_16
    jmp $

.io_read_disk_sector_count_error:
    mov bx, MSG_IO_DISK_SECTOR_COUNT_ERROR_MSG
    call print_str_16
    jmp $