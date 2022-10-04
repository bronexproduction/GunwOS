;
;  print_32.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 15.01.2018.
;

BITS 32

VGA_MEM             equ 0xb8000
WHITE_ON_BLACK      equ 0x0f
LINES               equ 25
COLUMNS             equ 80
BYTES_PER_CHAR      equ 2
VGA_MEM_END         equ VGA_MEM + (LINES * COLUMNS * BYTES_PER_CHAR)

print_str_32_cur_line_offset:
    dd VGA_MEM

; Prints string (32-bit protected mode only)
;
; Params:
; EBX - string memory location

print_str_32:
    pushad
    mov edx, [print_str_32_cur_line_offset]

.print_str_char_32:
    mov al, [ebx]
    mov ah, WHITE_ON_BLACK
    cmp al, 0
    je .print_str_end_32
    mov [edx], ax
    add ebx, 1
    add edx, BYTES_PER_CHAR

    cmp edx, VGA_MEM_END
    jl .print_str_char_32

    mov edx, VGA_MEM

    jmp .print_str_char_32

.print_str_end_32:

    ; ---------------------------------------
    ; Move to next line
    ; ---------------------------------------

    mov ebx, edx
    mov eax, edx
    sub eax, VGA_MEM
    xor edx, edx
    mov ecx, COLUMNS * BYTES_PER_CHAR
    div ecx

    cmp edx, 0
    je .print_str_end_checkLimit_32

    sub ebx, edx
    add ebx, COLUMNS * BYTES_PER_CHAR
    
.print_str_end_checkLimit_32:

    cmp ebx, VGA_MEM_END
    jl .print_str_exit_32

    mov ebx, VGA_MEM

.print_str_exit_32:
    mov [print_str_32_cur_line_offset], ebx
    popad
    ret
