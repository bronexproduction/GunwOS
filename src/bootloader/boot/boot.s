;
;  boot.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 08.01.2018.
;

ORG 0x7c00

BITS 16

RMODE_STACK_ADDR            equ 0x700
FAT_HEADER_ADDR             equ 0x1000
SECOND_STAGE_SEG            equ 0x1000

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
    mov bp, RMODE_STACK_ADDR
    mov sp, bp

    mov bx, BOOT_LOAD_MSG
    call print_str_16

boot_load_2nd_stage:
    ; ---------------------------------------
    ; Fetch 2nd stage loader from filesystem 
    ; ---------------------------------------
    mov ax, BOOT_2ND_STAGE_FILENAME
    mov bx, FAT_HEADER_ADDR
    mov cx, SECOND_STAGE_SEG
    mov dx, 0
    call fat12_loadFile

    mov bx, DEBUG_LOAD_SUCCESS
    call print_str_16
    jmp $

boot_2nd_stage:
    ; ---------------------------------------
    ; Jump to preloader (2-nd stage)
    ; ---------------------------------------
    mov bx, BOOT_PRELOADER_MSG
    call print_str_16

    jmp SECOND_STAGE_SEG
    jmp $

    ; ---------------------------------------
    ; Additional routines and utilities
    ; ---------------------------------------

%include "print.s"
%include "io.s"
%include "../shared/fat12.s"

BOOT_2ND_STAGE_FILENAME db "BOOT    GFB"

BOOT_LOAD_MSG db 'GunwOS loading...', 0xa, 0xd, 0
BOOT_PRELOADER_MSG db 'GunwOS preloader booting...', 0xa, 0xd, 0

DEBUG_LOAD_SUCCESS db 'File loaded successfully - stopped', 0xa, 0xd, 0

boot_fill:
    times 510-($-$$) db 0

    ; ---------------------------------------
    ; FAT reserved sectors
    ;
    ; Boot sector signature
    ; ---------------------------------------
    dw 0xaa55
