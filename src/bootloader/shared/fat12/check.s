
;
;  check.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 08.07.2022.
;

    ; ---------------------------------------
    ; Check if sector valid for read
    ; 
    ; AX - sector num
    ;
    ; Result - CF set if not valid
    ; ---------------------------------------

fat12_checkValidForRead:

    pusha

    mov bx, 2
    mov cx, FAT12_MAX_READABLE_CLUSTER

    call fat12_checkRange
    
    popa
    ret

    ; ---------------------------------------
    ; Check if sector if EOF
    ; 
    ; AX - sector num
    ;
    ; Result - CF set if not EOF
    ; ---------------------------------------

fat12_checkEOF:

    pusha

    mov bx, 0xFF8
    mov cx, 0xFFF

    call fat12_checkRange
    
    popa
    ret

    ; ---------------------------------------
    ; Check if value in range
    ; 
    ; AX - value
    ; BX - range bottom limit (inclusive)
    ; CX - range top limit (inclusive)
    ;
    ; Result - CF set if not in range
    ; ---------------------------------------

fat12_checkRange:

    pusha

    cmp ax, bx
    jl .fat12_checkRange_error

    cmp ax, cx
    jg .fat12_checkRange_error

    clc
    jmp .fat12_checkRange_end

.fat12_checkRange_error:

    stc

.fat12_checkRange_end:

    popa
    ret