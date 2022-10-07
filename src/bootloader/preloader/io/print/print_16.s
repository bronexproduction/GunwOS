;
;  print_16.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 15.01.2018.
;

BITS 16

; Prints string (16-bit real mode only)
;
; Params:
; BX - string memory location

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

; Prints byte hexadecimal value
;
; Params:
; AL - byte to be printed

print_hex_byte:
    pusha

    mov bx, PRINT_HEX_PREFIX
    call print_str_16

    popa

print_hex_byte_raw:
    pusha

    mov bh, 0
    mov bl, al              ; get upper byte
    shr bl, 4  
    shl bl, 1             
    add bx, print_data_hex_matrix
    call print_str_16

    mov bh, 0
    mov bl, al              ; get lower byte
    and bl, 0x0F
    shl bl, 1        
    add bx, print_data_hex_matrix
    call print_str_16

    popa
    ret
    
; Prints word hexadecimal value
;
; Params:
; AX - word to be printed

print_hex_word:
    pusha
    
    mov bx, PRINT_HEX_PREFIX
    call print_str_16

    popa

print_hex_word_raw:
    pusha
    mov bx, ax
    shr ax, 8
    call print_hex_byte_raw
    mov al, bl
    call print_hex_byte_raw

    popa
    ret

; Prints double word hexadecimal value
;
; Params:
; EAX - double word to be printed

print_hex_dword:
    pushad

    mov bx, PRINT_HEX_PREFIX
    call print_str_16

    popad

print_hex_dword_raw:
    pushad

    mov ebx, eax
    shr eax, 16
    call print_hex_word_raw
    mov ax, bx
    call print_hex_word_raw

    popad
    ret

PRINT_HEX_PREFIX db '0x', 0

print_data_hex_matrix:
    db '0', 0
    db '1', 0
    db '2', 0
    db '3', 0
    db '4', 0
    db '5', 0
    db '6', 0
    db '7', 0
    db '8', 0
    db '9', 0
    db 'a', 0
    db 'b', 0
    db 'c', 0
    db 'd', 0
    db 'e', 0
    db 'f', 0