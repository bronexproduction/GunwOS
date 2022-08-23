;
;  read_kernel.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 04.01.2021.
;

BITS 16

; Read kernel binary

read_kernel:
    pusha

    call io_read_drive_parameters                   ; read drive parameters
    
    mov bx, MSG_BOOT_DRIVE              
    call print_str_16

    mov bx, MSG_BOOT_DRIVE_INDEX
    call print_str_16
    mov al, [io_data_drive_index]
    call print_hex_byte
    mov bx, MSG_IO_NEWLINE
    call print_str_16

    mov bx, MSG_BOOT_DRIVE_SECTORS_PER_TRACK
    call print_str_16
    mov al, [io_data_drive_sectors_per_track]
    call print_hex_byte
    mov bx, MSG_IO_NEWLINE
    call print_str_16

    mov bx, MSG_BOOT_DRIVE_CYLINDERS
    call print_str_16
    mov ax, [io_data_drive_cylinders]
    call print_hex_word
    mov bx, MSG_IO_NEWLINE
    call print_str_16

    mov bx, MSG_BOOT_DRIVE_HEADS
    call print_str_16
    mov al, [io_data_drive_heads]
    call print_hex_byte
    mov bx, MSG_IO_NEWLINE
    call print_str_16

    ; ---------------------------------------
    ; Fetch kernel from filesystem 
    ; ---------------------------------------
    mov ax, BOOT_KERNEL_FILENAME
    mov si, FAT_HEADER_ADDR
    mov di, KERNEL_SEG
    call fat12_loadFile

read_kernel_success:
    mov bx, MSG_KERNEL_LOAD_SUCCESS
    call print_str_16

read_kernel_end:
    popa
    ret
