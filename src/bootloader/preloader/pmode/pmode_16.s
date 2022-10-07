;
;  pmode_16.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 28.09.2022.
;

BITS 16

    jmp pmode_switch

%include "pmode/gdt.s"

pmode_switch:

    cli
    lgdt [gdt_desc]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    
    jmp CODE_SEG:pmode_init     ; long jump to prevent CPU from prefetching during mode change
