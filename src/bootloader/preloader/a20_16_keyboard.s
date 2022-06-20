;
;  a20_16_keyboard.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 03.02.2021.
;

BITS 16

; Tries to check/enable A20 using 8042 keyboard controller
; Note: Use in real mode only

a20_16_keyboard:

    call a20_16_keyboard_wait_command   ; When controller ready for command
    mov al, 0xad                        ; Send command 0xad (disable keyboard).
    out 0x64, al

    call a20_16_keyboard_wait_command   ; When controller ready for command
    mov al, 0xd0                        ; Send command 0xd0 (read from input)
    out 0x64, al

    call a20_16_keyboard_wait_data      ; When controller has data ready
    in al, 0x60                         ; Read input from keyboard
    push ax                             ; ... and save it

    call a20_16_keyboard_wait_command   ; When controller is ready for command
    mov al, 0xd1                        ; Set command 0xd1 (write to output)
    out 0x64, al            

    call a20_16_keyboard_wait_command   ; When controller is ready for command
    pop ax                              ; Write input back, with bit #2 set
    or al, 2
    out 0x60, al

    call a20_16_keyboard_wait_command   ; When controller is ready for command
    mov al, 0xae                        ; Write command 0xae (enable keyboard)
    out 0x64, al

    call a20_16_keyboard_wait_command   ; Wait until controller is ready for command

    jmp a20_16_keyboard_finish

a20_16_keyboard_wait_command:
    xor bx, bx
a20_16_keyboard_wait_command_loop:
    inc bx
    jz a20_16_keyboard_timeout

    in al, 0x64
    cmp al, 2
    jnz a20_16_keyboard_wait_command_loop
    ret

a20_16_keyboard_wait_data:
    xor bx, bx
a20_16_keyboard_wait_data_loop:
    inc bx
    jz a20_16_keyboard_timeout

    in al, 0x64
    cmp al, 1
    jz a20_16_keyboard_wait_data_loop
    ret

a20_16_keyboard_finish:
    mov bx, MSG_A20_KEYBOARD_END
    call print_str_16
    jmp a20_16_keyboard_end

a20_16_keyboard_timeout:
    mov bx, MSG_A20_KEYBOARD_TIMEOUT
    call print_str_16
    jmp a20_16_keyboard_end
    
a20_16_keyboard_end:
