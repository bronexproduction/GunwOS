;
;  print.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 15.01.2018.
;

BITS 16

print_err_16:
    mov ah, 0x0e
    mov si, ERROR_STRING

.print_err_16_char:
    cmp bx, si
    jl .print_err_16_end
    lodsb
    int 0x10
    jmp .print_err_16_char

.print_err_16_end:
    cli
    hlt
