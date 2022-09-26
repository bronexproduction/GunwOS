;
;  preloader.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 03.01.2021.
;

ORG 0x10000

BITS 16

jmp preloader_start

%include "data.s"
%include "fat_space.s"

BITS 16

preloader_start:

    ; ---------------------------------------
    ; Initialize data segment
    ; with code segment address
    ; ---------------------------------------
    mov bx, cs
    mov ds, bx

    mov bx, MSG_PRELOADER_START
    call print_str_16

    ; ---------------------------------------
    ; Try to activate A20 line
    ; ---------------------------------------
    call a20_16

    ; ---------------------------------------
    ; Load kernel binary
    ;
    ; NOTE:
    ; Binary is expected to fit
    ; in 0x20000 - 0x9FFFF address range
    ; (starting from 0x2000:0x0 up to 640K)
    ; Which gives 512K for kernel binary
    ; ---------------------------------------
    mov bx, cs
    mov es, bx
    call read_kernel

    ; ---------------------------------------
    ; Jump to protected mode
    ; ---------------------------------------
%include "pmode/pmode.s"

    jmp $

    ; ---------------------------------------
    ; Verify A20 line in protected mode
    ; ---------------------------------------
%include "a20/a20_32.s"

BITS 32

    mov ebx, MSG_KERNEL_START
    call print_str_32

    ; ---------------------------------------
    ; Start kernel
    ; ---------------------------------------
%include "kernel/kernel_data.s"
    mov ecx, kernel_data    ; __kernel_start will receive kernel_data pointer as parameter (fastcall)
    call KERNEL_SEG:0

    ; ---------------------------------------
    ; Halt the CPU
    ;
    ; NOTE:
    ; Just in case kernel entry
    ; fails for some reason
    ; ---------------------------------------
    cli
    hlt

%include "a20/a20_16.s"
%include "io.s"
%include "print_16.s"
%include "print_32.s"
%include "msg.s"
%include "read_kernel.s"
%include "../shared/fat12/fat12.s"
%include "ioerr.s"
%include "fat12err.s"

BOOT_KERNEL_FILENAME                db "KERNEL  GFB"
FAT12_READ_FILE_SIZE_LIMIT_BYTES    equ 524288 
