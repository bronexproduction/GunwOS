;
;  boot.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 08.01.2018.
;

ORG 0x7c00

BITS 16

jmp boot_init

%include "../shared/data.s"

BITS 16

boot_init:
    mov ax, 0x00
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov bp, RMODE_STACK
    mov sp, bp

    mov [preloader_data_bootDrive], dl

    mov bx, BOOT_LOAD_MSG
    call print_str_16

boot_load_prep:
    mov cl, BOOT_LOAD_SECTOR_BEGIN    ; first sector after boot loader

boot_load:
    mov bx, PRELOADER_MEM_OFFSET
    mov dh, BOOT_LOAD_SECTOR_COUNT
    mov dl, [preloader_data_bootDrive]
    call io_read_disk

boot_check_entry:
    mov eax, PRELOADER_MEM_OFFSET
    mov ebx, [eax]
    cmp ebx, ELF_HEADER
    jne boot_preloader

boot_elf:
    mov bx, BOOT_LOAD_DEBUG_ENTRY
    call print_str_16
    
boot_offset:
    mov cl, BOOT_LOAD_SECTOR_BEGIN
    add cl, BOOT_LOAD_SECTOR_ELF_OFFSET
    jmp boot_load

boot_preloader:
    mov bx, BOOT_PRELOADER_MSG
    call print_str_16

    mov cx, preloader_data     ; preloader_start will receive preloader_data pointer as parameter (fastcall)
    jmp PRELOADER_MEM_OFFSET
    jmp $

%include "preloader_data.s"
%include "print.s"
%include "io.s"

BOOT_LOAD_MSG db 'GunwOS loading...', 0xa, 0xd, 0
BOOT_LOAD_DEBUG_ENTRY db 'ELF header found. Offsetting...', 0xa, 0xd, 0
BOOT_PRELOADER_MSG db 'GunwOS preloader booting...', 0xa, 0xd, 0

boot_fill:
    times 510-($-$$) db 0

    dw 0xaa55               ; boot sector magic number
