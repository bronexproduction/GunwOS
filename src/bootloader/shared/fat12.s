;
;  fat12.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 06.07.2022.
;

BITS 16

FAT12_DIR_ENTRY_BYTES           equ 32
FAT12_FILENAME_FULL_BYTES       equ 11
FAT12_DIR_ENTRIES_PER_SECTOR    equ (BPB_BYTES_PER_LOGICAL_SECTOR / FAT12_DIR_ENTRY_BYTES)
FAT12_ROOT_DIR_SECTORS          equ (BPB_MAX_ROOT_DIRECTORY_ENTRIES / FAT12_DIR_ENTRIES_PER_SECTOR)
FAT12_ROOT_DIR_BYTES            equ (FAT12_ROOT_DIR_SECTORS * BPB_BYTES_PER_LOGICAL_SECTOR)
FAT12_FATS_SECTORS              equ (BPB_NUMBER_OF_FATS * BPB_LOGICAL_SECTORS_PER_FAT)
FAT12_FAT_BYTES                 equ (BPB_LOGICAL_SECTORS_PER_FAT * BPB_BYTES_PER_LOGICAL_SECTOR)
FAT12_FATS_BYTES                equ (BPB_NUMBER_OF_FATS * FAT12_FAT_BYTES)
FAT12_HEADER_SECTORS            equ (FAT12_FATS_SECTORS + FAT12_ROOT_DIR_SECTORS)

    ; ---------------------------------------
    ; Load file from FAT12 disk
    ; 
    ; AX - address of 11-bytes long filename
    ; BX - address of 16K unused memory
    ;      block to store FAT12 header
    ; CX - segment of read buffer (segment)
    ; DL - disk number
    ; ---------------------------------------

fat12_loadFile:

    ; Read FAT header sectors from the disk
    call fat12_loadHeader

    ; Verify FAT integrity
    call fat12_verifyFATs

    ; Find root directory entry for the file
    call fat12_findDir


    ret
    
    ; ---------------------------------------
    ; Load FAT header from FAT12 disk
    ; 
    ; BX - address of unused memory
    ;      block to store FAT12 header
    ;      (usually 16K for standard floppy)
    ; DL - disk number
    ; ---------------------------------------

fat12_loadHeader:

    pusha

    mov al, FAT12_HEADER_SECTORS
    mov cx, 2
    call io_read_disk

    popa
    ret

    ; ---------------------------------------
    ; Verify loaded FAT tables
    ; 
    ; BX - address of FAT header
    ; ---------------------------------------

fat12_verifyFATs:

    pusha 

    ; Set last byte offset as starting point
    mov cx, FAT12_FAT_BYTES
    mov dx, bx

.fat12_verifyFATs_check:

    dec cx

    ; Get value from first FAT
    mov bx, dx
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

    mov bx, FAT12_HEADER_INVALID_ERR_STRING
    call print_str_16
    jmp $

    ; ---------------------------------------
    ; Find root directory entry
    ; 
    ; AX - address of 11-bytes long filename
    ; BX - address of FAT header
    ; ---------------------------------------

fat12_findDir:

    pusha

    mov cx, FAT12_ROOT_DIR_BYTES
    mov dx, bx

.fat12_findDir_entry:

    ; move to previous entry
    sub cx, FAT12_DIR_ENTRY_BYTES
    mov bx, dx
    add bx, FAT12_FATS_BYTES
    add bx, cx
    
    ; compare filenames
    mov si, ax
    mov di, bx

    dec di

    push ax
    push cx

    mov cx, FAT12_FILENAME_FULL_BYTES
    inc cx

.fat12_findDir_checkChar:

    dec cx
    jz .fat12_findDir_checkChar_end
    
    inc di
    lodsb

    cmp [di], al

    je .fat12_findDir_checkChar

.fat12_findDir_checkChar_end:

    pop cx
    pop ax
    
    ; check if succeeded 
    jz .fat12_findDir_entryFound

    ; check if first entry checked (started from end)
    cmp cx, 0
    jne .fat12_findDir_entry

.fat12_findDir_entryNotFound:

    mov bx, FAT12_LOADER_NOT_FOUND_STRING
    call print_str_16
    jmp $

.fat12_findDir_entryFound:

    mov bx, BINGO
    call print_str_16

    ; How to return result? TODO

    jmp $

.fat12_findDir_end:

    popa
    ret

    ; ---------------------------------------
    ; Text resources
    ; ---------------------------------------

FAT12_HEADER_INVALID_ERR_STRING db 'Panic: FAT header invalid', 0
FAT12_LOADER_NOT_FOUND_STRING db 'Panic: Loader not found', 0
BINGO db 'Bingo', 0