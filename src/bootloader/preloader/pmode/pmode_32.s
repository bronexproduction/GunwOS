;
;  pmode_32.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 28.09.2022.
;

BITS 32

pmode_init:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, PMODE_STACK
    mov esp, ebp

    xor esi, esi
    xor edi, edi
    xor eax, eax
    xor ebx, ebx
    xor ecx, ecx
    xor edx, edx

    mov ebx, MSG_PMODE_SWITCH
    call print_str_32
