;
;  print_32.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 15.01.2018.
;

BITS 32

VGA_MEM equ 0xb8000
WHITE_ON_BLACK equ 0x0f

; Prints string (32-bit protected mode only)
;
; Params:
; EBX - string memory location

print_str_32:
    pushad
    mov edx, VGA_MEM

.print_str_char_32:
    mov al, [ebx]
    mov ah, WHITE_ON_BLACK
    cmp al, 0
    je .print_str_end_32
    mov [edx], ax
    add ebx, 1
    add edx, 2
    jmp .print_str_char_32

.print_str_end_32:
    popad
    ret
