;
;  preloader_16.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 28.09.2022.
;

BITS 16

jmp preloader_start

%include "data/data_16.s"
%include "io/print/print_16.s"

%include "a20/a20_16.s"
%include "read_kernel.s"

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

%include "pmode/pmode_16.s"
