;
;  a20_16_interrupt.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 07.01.2021.
;

BITS 16

; Tries to check/enable A20 using BIOS extension (SYSTEM - later PS/2s)
; Note: Use in real mode only

a20_16_interrupt:
    mov ax, 2403h                       ; QUERY A20 GATE SUPPORT
    int 0x15
    jc a20_16_interrupt_not_supported   ; if CF set - INT 15h is not supported
    cmp ah, 0                           ; AH contains 0 or error status
    jnz a20_16_interrupt_not_supported  ; if error - not supported
 
    mov ax, 2402h                       ; GET A20 GATE STATUS
    int 0x15
    jc a20_16_interrupt_failed          ; if CF set - error
    cmp ah, 0                           ; AH contains 0 or error status
    jnz a20_16_interrupt_failed         ; if error - failed
 
    cmp al, 1
    je a20_16_already_activated         ; A20 already activated
 
a20_16_interrupt_activate:
    mov ax, 2401h                       ; ENABLE A20 GATE
    int 15h
    jc a20_16_interrupt_failed          ; if CF set - activation failed
    cmp ah, 0                           ; AH contains 0 or error status
    jnz a20_16_interrupt_failed         ; if error - gate activation failed

a20_16_interrupt_activated:
    mov bx, MSG_A20_15H_ACTIVATED
    call print_str_16
    jmp a20_16_end
    
a20_16_already_activated:
    mov bx, MSG_A20_15H_ALREADY_ACTIVATED
    call print_str_16
    jmp a20_16_end

a20_16_interrupt_not_supported:
    mov bx, MSG_A20_15H_NOT_SUPPORTED
    call print_str_16
    jmp a20_16_interrupt_end

a20_16_interrupt_failed:
    mov bx, MSG_A20_15H_FAILED
    call print_str_16
    jmp a20_16_interrupt_end

a20_16_interrupt_end:
