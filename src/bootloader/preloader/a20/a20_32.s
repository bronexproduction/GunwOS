;
;  a20_32.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 20.01.2018.
;

BITS 32

; Checking access to extended memory
;
; Checks if the memory wraps over 1MB limit to ensure access to extended memory

a20_32_check:
    pushad

    mov ecx, 0                  ; A20 enable retries count

    mov eax, A20_CHECK_OFFSET
    mov ebx, 0x100000

    mov esi, eax                ; make ESI an conventional memory address (over FFF0h, writable)
    add eax, ebx
    mov edi, eax                ; make EDI an extended memory address (esi + 1MB)

a20_32_check_overlap:
    mov eax, [esi]              ; backup original conventional memory content

    mov ebx, [edi]              ; copy content of extended memory
    inc ebx                     ; increment
    mov [esi], ebx              ; write to conventional memory

    cmpsd                       ; compare addresses to see if the're equivalent
    jne a20_32_ok               ; if not equivalent - the A20 line is set.

a20_32_enable:
    mov [esi], eax              ; restore original conventional memory content

    cmp ecx, 0
    jnz a20_32_error            ; if retry - fail

    ; ACTIVATE A20

    inc ecx                     ; increment retry count
    jmp a20_32_check_overlap    ; check again

a20_32_error:
    mov ebx, MSG_A20_DISABLED_ERROR
    call print_str_32
    cli
    hlt

a20_32_ok:
    mov [esi], eax              ; restore original conventional memory content
    
    mov ebx, MSG_A20_ENABLED
    call print_str_32

    popad
