;
;  preloader_32.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 28.09.2022.
;

BITS 32

%include "data/data_32.s"
%include "io/print/print_32.s"

    ; ---------------------------------------
    ; Initialize protected mode
    ; ---------------------------------------

%include "pmode/pmode_32.s"

    ; ---------------------------------------
    ; Verify A20 line in protected mode
    ; ---------------------------------------

%include "a20/a20_32.s"

    mov ebx, MSG_KERNEL_START
    call print_str_32

    ; ---------------------------------------
    ; Start kernel
    ; ---------------------------------------

%include "kernel/kernel_data.s"

    mov ecx, kernel_data    ; __kernel_start will receive kernel_data pointer as parameter (fastcall)
    call KERNEL_ADDR
