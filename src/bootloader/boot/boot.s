;
;  boot.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 08.01.2018.
;

ORG 0x7c00

BITS 16

RMODE_STACK                 equ 0x700
PRELOADER_MEM_OFFSET        equ 0x1000
ELF_HEADER                  equ 0x464c457f
%include "../shared/data.s"

jmp boot_init

    ; ---------------------------------------
    ; IMPORTANT NOTE
    ; 
    ; DL register contains boot drive number
    ; try not to overwrite it
    ; ---------------------------------------

    ; ---------------------------------------
    ; FAT reserved sectors
    ; 
    ; OEM name
    ; ---------------------------------------
oem_name:   db "GunwOS  "
    
    ; ---------------------------------------
    ; FAT reserved sectors
    ; 
    ; BIOS parameter block
    ; ---------------------------------------
%include "bpb.s"

BITS 16

boot_init:
    ; ---------------------------------------
    ; Clear accumulator and segment registers
    ; ---------------------------------------
    mov ax, 0x00
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; ---------------------------------------
    ; Configure stack
    ; ---------------------------------------
    mov bp, RMODE_STACK
    mov sp, bp

    mov bx, BOOT_LOAD_MSG
    call print_str_16

boot_load_prep:
    ; ---------------------------------------
    ; Start reading from first sector
    ; after boot loader
    ; ---------------------------------------
    mov cl, BOOT_LOAD_SECTOR_BEGIN

boot_load:
    ; ---------------------------------------
    ; Read BOOT_LOAD_SECTOR_COUNT
    ; Into PRELOADER_MEM_OFFSET
    ; Starting from first sector after boot loader
    ; ---------------------------------------
    mov bx, PRELOADER_MEM_OFFSET
    mov dh, BOOT_LOAD_SECTOR_COUNT
    call io_read_disk

boot_check_entry:
    ; ---------------------------------------
    ; Check for ELF header in loaded file
    ; ---------------------------------------
    mov eax, PRELOADER_MEM_OFFSET
    mov ebx, [eax]
    cmp ebx, ELF_HEADER
    jne boot_preloader

boot_elf:
    ; ---------------------------------------
    ; Print message on ELF header detected
    ; ---------------------------------------
    mov bx, BOOT_LOAD_DEBUG_ENTRY
    call print_str_16
    
boot_elf_offset:
    ; ---------------------------------------
    ; Offset loading to first sector
    ; after ELF header
    ; ---------------------------------------
    mov cl, BOOT_LOAD_SECTOR_BEGIN
    add cl, BOOT_LOAD_SECTOR_ELF_OFFSET
    jmp boot_load

boot_preloader:
    ; ---------------------------------------
    ; Jump to preloader (2-nd stage)
    ; ---------------------------------------
    mov bx, BOOT_PRELOADER_MSG
    call print_str_16

    jmp PRELOADER_MEM_OFFSET
    jmp $

    ; ---------------------------------------
    ; Additional routines and utilities
    ; ---------------------------------------

%include "print.s"
%include "io.s"

BOOT_LOAD_MSG db 'GunwOS loading...', 0xa, 0xd, 0
BOOT_LOAD_DEBUG_ENTRY db 'ELF header found. Offsetting...', 0xa, 0xd, 0
BOOT_PRELOADER_MSG db 'GunwOS preloader booting...', 0xa, 0xd, 0

boot_fill:
    times 510-($-$$) db 0

    ; ---------------------------------------
    ; FAT reserved sectors
    ;
    ; Boot sector signature
    ; ---------------------------------------
    dw 0xaa55
