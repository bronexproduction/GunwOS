;
;  a20_16.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 07.01.2021.
;

BITS 16

; Checking and enabling access to extended memory
;
; Checks if the memory wraps over 1MB limit to ensure access to extended memory

a20_16:
    call a20_16_check
    cmp al, 1
    je a20_16_end

%include "a20/a20_16_interrupt.s"

    call a20_16_check
    cmp al, 1
    je a20_16_end

%include "a20/a20_16_keyboard.s"

    call a20_16_check
    cmp al, 1
    je a20_16_end

%include "a20/a20_16_fast.s"

    call a20_16_check
    cmp al, 1
    je a20_16_end

a20_16_fail:
    mov bx, MSG_A20_DISABLED_ERROR
    call print_str_16
    jmp $

a20_16_check:
    pushf
    push ds
    push es
    push di
    push si
 
    xor ax, ax ; ax = 0
    mov es, ax
 
    not ax ; ax = 0xFFFF
    mov ds, ax
 
    mov di, 0x0500
    mov si, 0x0510
 
    mov al, byte [es:di]
    push ax
 
    mov al, byte [ds:si]
    push ax
 
    mov byte [es:di], 0x00
    mov byte [ds:si], 0xFF
 
    cmp byte [es:di], 0xFF
 
    pop ax
    mov byte [ds:si], al
 
    pop ax
    mov byte [es:di], al
 
    mov ax, 0                   ; disabled (memory wraps around)
    je a20_16_check_end         ; if equal then memory wraps around)
 
    mov ax, 1                   ; enabled (memory does not wrap around)
 
a20_16_check_end:
    pop si
    pop di
    pop es
    pop ds
    popf
 
    ret 

a20_16_end:
    mov bx, MSG_A20_ENABLED
    call print_str_16
