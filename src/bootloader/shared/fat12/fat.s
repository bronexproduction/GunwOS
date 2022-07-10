;
;  fat.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 08.07.2022.
;

    ; ---------------------------------------
    ; Verify loaded FAT tables
    ; 
    ; SI - address of FAT header
    ; ---------------------------------------

fat12_verifyFATs:

    pusha 

    ; Set last byte offset as starting point
    mov cx, FAT12_FAT_BYTES

.fat12_verifyFATs_check:

    dec cx

    ; Get value from first FAT
    mov bx, si
    add bx, cx
    mov al, [bx]

    ; Get value from second FAT
    add bx, FAT12_FAT_BYTES
    push ax
    mov al, [bx]
    pop bx

    ; Compare bytes
    cmp al, bl
    jne .fat12_verifyFATs_error

    ; Check if all bytes verified
    cmp cx, 0
    jne .fat12_verifyFATs_check

.fat12_verifyFATs_end:

    popa
    ret

.fat12_verifyFATs_error:

    mov bx, FAT12_INVALID_ERROR_STRING
    call print_err_16

    ; ---------------------------------------
    ; Get entry from FAT table
    ; 
    ; AX - current cluster
    ; SI - address of FAT header
    ;
    ; Result - AX - next cluster
    ; ---------------------------------------

fat12_getEntry:

    pusha

    popa
    ret
