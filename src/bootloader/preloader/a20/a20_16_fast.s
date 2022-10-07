;
;  a20_16_fast.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 03.02.2021.
;

BITS 16

; Tries to enable A20 using Fast A20 method
; Note: Use in real mode only and as a solution of the last resort 

a20_16_fast:

    in al, 0x92
    test al, 2
    jnz a20_16_fast_not_supported
    or al, 2
    and al, 0xfe
    out 0x92, al

a20_16_fast_finished:
    mov bx, MSG_A20_FAST_END
    call print_str_16
    jmp a20_16_fast_end

a20_16_fast_not_supported:
    mov bx, MSG_A20_FAST_NOT_SUPPORTED
    call print_str_16
    jmp a20_16_fast_end

a20_16_fast_end:    
