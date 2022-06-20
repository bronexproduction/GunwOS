;
;  mem.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 03.01.2021.
;

BITS 16

; Copies memory
;
; Params:
; AX - destination memory address
; BX - source memory address
; CX - destination memory offset
; DX - byte count

mem_copy:
    pusha
    push es

    mov es, cx              ; set data segment offset (????????)

mem_copy_move_byte:
    mov cx, [bx]            ; read byte from source
    push bx                 ; store BX as AX can't be used as an index register
    mov bx, ax              ; load destination address into BX

    push ax                 ; store AX as it will be used to pass offset to DS
    push ds                 ; store DS as it will be updated
    
    mov ax, es              ; read stored segment offset
    mov ds, ax              ; set DS

    mov [bx], cx            ; move byte to destination address

    pop ds                  ; reload DS
    pop ax                  ; reload AX
    pop bx                  ; reload BX

mem_copy_increment:
    inc ax                  ; set next byte for source
    inc bx                  ; set next byte for destination

mem_copy_decrement_count:
    dec dx                  ; less bytes to be read
    jnz mem_copy_move_byte  ; if DX != 0 - read next byte
    
mem_copy_end:
    pop es
    popa
    ret

; 4 byte pattern verification
;
; Params:
; EAX - pattern
; BX - memory address
; CX - length in bytes (32-bit multiple)
;
; Result:
; CF: 0 - success
;     1 - error
; AL: 0x00 - pattern matching
;     0x01 - pattern not-matching
;     0x02 - parameter error

mem_verify_4b_pattern:
    pusha

    mov dx, cx                                      ; check if CX correct (4-byte alignment)
    and dx, 0x0003                      
    jnz mem_verify_4b_pattern_failure_param_error   ; if not - error

    shr cx, 2                                       ; divide byte count by 4 to get iteration count

mem_verify_4b_pattern_check_pattern:                                                
    cmp cx, 0                                       ; if finished without error - verification successful
    je mem_verify_4b_pattern_success_matching

    mov edx, [bx]                                   ; copy memory content
    cmp eax, edx                                    ; compare with pattern
    jne mem_verify_4b_pattern_success_non_matching  ; if does not match - success with not-matching result

    add bx, 4                                       ; offset memory by 4 bytes
    dec cx                                          ; decrement iteration count
    jmp mem_verify_4b_pattern_check_pattern         ; check pattern at the next address

mem_verify_4b_pattern_failure_param_error:
    popa
    mov al, 0x02
    stc
    jmp mem_verify_4b_pattern_end

mem_verify_4b_pattern_success_non_matching:
    popa
    mov al, 0x01
    clc
    jmp mem_verify_4b_pattern_end

mem_verify_4b_pattern_success_matching:
    popa
    mov al, 0x00
    clc
    jmp mem_verify_4b_pattern_end

mem_verify_4b_pattern_end:
    ret
