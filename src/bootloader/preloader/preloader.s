;
;  preloader.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 03.01.2021.
;

ORG 0x10000

%include "data.s"

BITS 16

jmp preloader_start

BITS 16

preloader_start:

    mov bx, MSG_PRELOADER_START
    call print_str_16

%include "a20/a20_16.s"

    call read_kernel

    jmp pmode_switch
%include "pmode/pmode.s"
%include "a20/a20_32.s"

BITS 32

    mov ebx, MSG_KERNEL_START
    call print_str_32

boot_kernel:
%include "kernel/kernel_data.s"
    mov ecx, kernel_data    ; __kernel_start will receive kernel_data pointer as parameter (fastcall)
    call KERNEL_SEG:0
    jmp $


%include "io.s"
%include "print_16.s"
%include "print_32.s"
%include "mem.s"
%include "msg.s"
%include "read_kernel.s"
%include "../shared/fat12/fat12.s"
%include "fat12err.s"

BOOT_KERNEL_FILENAME db "KERNEL  GFB"
