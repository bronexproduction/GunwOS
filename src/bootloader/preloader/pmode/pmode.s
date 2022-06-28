;
;  pmode.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 15.01.2018.
;

BITS 16

%include "pmode/gdt.s"

pmode_switch:
    cli
    lgdt [gdt_desc]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:pmode_init     ; long jump to prevent CPU from prefetching during mode change

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
