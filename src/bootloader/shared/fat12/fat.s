;
;  fat.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 08.07.2022.
;

    ; ---------------------------------------
    ; Verify loaded FAT tables
    ; ---------------------------------------

fat12_verifyFATs:

    pusha 

    ; Set last byte offset as starting point
    mov cx, FAT12_FAT_BYTES

.fat12_verifyFATs_check:

    dec cx

    ; Get value from first FAT
    mov bx, FAT_HEADER_ADDR
    add bx, cx
    mov al, [bx]

    ; Get value from second FAT
    add bx, FAT12_FAT_BYTES

    ; Compare bytes
    cmp al, [bx]
    jne fat12_err_fatValidationFailed

    ; Check if all bytes verified
    cmp cx, 0
    jne .fat12_verifyFATs_check

.fat12_verifyFATs_end:

    popa
    ret

    ; ---------------------------------------
    ; Get entry from FAT table
    ; 
    ; AX - current cluster
    ;
    ; Result - AX - next cluster
    ; ---------------------------------------

fat12_getEntry:

    pusha

    mov bx, 3
    mul bx
    shr ax, 1 ; index
    pushf

    mov bx, ax
    add bx, FAT_HEADER_ADDR
    mov ax, [bx]

    popf
    jnc .fat12_getEntry_even

    shr ax, 4
    jmp .fat12_getEntry_end

.fat12_getEntry_even:
    
    and ax, 0x0FFF

.fat12_getEntry_end:

    ; Replace AX on stack
    mov di, sp
    add di, 14
    mov [di], ax

    popa
    ret
