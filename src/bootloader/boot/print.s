;
;  print.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 15.01.2018.
;

BITS 16

print_str_16:
    pusha
    mov ah, 0x0e
    mov si, bx

.print_str_16_char:
    lodsb
    cmp al, 0
    je .print_str_16_end
    int 0x10
    jmp .print_str_16_char

.print_str_16_end:
    popa
    ret
