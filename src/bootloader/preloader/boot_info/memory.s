;
;  memory.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 08.05.2024.
;

BITS 16

; Detect lower memory

detect_lower_memory:
    pusha
    clc
    int 0x12
    jc .detect_lower_memory_fail

.detect_lower_memory_end:
    mov [CONV_MEM_AVAILABLE], ax
    popa
    ret

.detect_lower_memory_fail:
    mov bx, MSG_MEMORY_ERROR_DETECTION_CONV_MEM
    call print_str_16
    cli
    hlt

; Detect upper memory

; INT 15h, AX=E801h or INT 15h AH=88h to be implemented as fallback options
; http://www.uruk.org/orig-grub/mem64mb.html

E820_PRESENT    db 0
detect_upper_memory:
    pusha
    clc
    mov ax, 0
    mov es, ax
    mov di, kernel_data_e820_entries
    xor ebx, ebx
    mov edx, 'PAMS'

.detect_upper_memory_loop:
    ; ---------------------------------------
    ; Check entries array boundary
    ; ---------------------------------------
    mov cx, di
    add cx, E820_ENTRY_BYTES
    cmp cx, kernel_data_e820_entries_end
    jg .detect_upper_memory_fail
    
    mov ecx, E820_ENTRY_BYTES
    mov eax, 0xE820
    int 0x15

    ; ---------------------------------------
    ; Check carry flag
    ; ---------------------------------------
    jc .detect_upper_memory_fail

    ; ---------------------------------------
    ; Check 'SMAP' value presence
    ; ---------------------------------------
    cmp eax, edx
    jne .detect_upper_memory_fail

    ; ---------------------------------------
    ; Check returned buffer size
    ; (20 - E820_ENTRY_BYTES)
    ; ---------------------------------------
    cmp ecx, 20
    jl .detect_upper_memory_fail
    cmp ecx, E820_ENTRY_BYTES
    jg .detect_upper_memory_fail
    
    ; ---------------------------------------
    ; No fail detected - mark E820 as present
    ; ---------------------------------------
    mov byte [E820_PRESENT], 1

    ; ---------------------------------------
    ; Check continuation value presence
    ; ---------------------------------------
    cmp ebx, 0
    je .detect_upper_memory_end

    ; ---------------------------------------
    ; Offset buffer to the next item
    ; ---------------------------------------
    add di, E820_ENTRY_BYTES
    jmp .detect_upper_memory_loop

.detect_upper_memory_end:
    popa
    ret

.detect_upper_memory_fail:
    cmp byte [E820_PRESENT], 1
    jne .detect_upper_memory_fallback

    mov bx, MSG_MEMORY_ERROR_DETECTION_UPPER_MEM_FAIL
    call print_str_16
    cli
    hlt

.detect_upper_memory_fallback:
    mov bx, MSG_MEMORY_DETECTION_NO_FALLBACK_IMPL
    call print_str_16
    cli
    hlt