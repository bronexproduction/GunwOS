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
SECOND_STAGE_SEG            equ 0x1000
%include "../shared/data.s"

BITS 16

    jmp boot_init
    times 3-($-$$) db 0

    ; ---------------------------------------
    ; IMPORTANT NOTE
    ; 
    ; DL register contains boot drive number
    ; remember not to overwrite it
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
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov es, ax

    ; ---------------------------------------
    ; Configure stack
    ; ---------------------------------------
    mov bp, RMODE_STACK_ADDR
    mov sp, bp

boot_load_2nd_stage:
    ; ---------------------------------------
    ; Fetch 2nd stage loader from filesystem 
    ; ---------------------------------------
    mov ax, BOOT_2ND_STAGE_FILENAME
    mov si, FAT_HEADER_ADDR
    mov di, SECOND_STAGE_SEG
    call fat12_loadFile

boot_2nd_stage:
    ; ---------------------------------------
    ; Jump to preloader (2-nd stage)
    ; ---------------------------------------
    jmp SECOND_STAGE_SEG:0

    ; ---------------------------------------
    ; Additional routines and utilities
    ; ---------------------------------------

%include "print.s"
%include "../shared/io.s"
%include "../shared/fat12/fat12.s"
%include "ioerr.s"
%include "fat12err.s"
%include "msg.s"

BOOT_2ND_STAGE_FILENAME db "BOOT    GFB"

boot_fill:
    times 510-($-$$) db 0

    ; ---------------------------------------
    ; FAT reserved sectors
    ;
    ; Boot sector signature
    ; ---------------------------------------
    dw 0xaa55
