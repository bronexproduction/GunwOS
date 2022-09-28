;
;  dir.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 08.07.2022.
;

    ; ---------------------------------------
    ; Find root directory entry
    ; 
    ; AX - address of 11-bytes long filename
    ; SI - address of FAT header
    ;
    ; Result - BX - address of dir entry
    ; ---------------------------------------

fat12_findDir:

    pusha

    mov cx, FAT12_ROOT_DIR_BYTES

.fat12_findDir_entry:

    ; move to previous entry
    sub cx, FAT12_DIR_ENTRY_BYTES
    mov bx, si
    add bx, FAT12_FATS_BYTES
    add bx, cx
    
    ; compare filenames
    pusha

    mov si, ax
    mov di, bx

    dec di

    mov cx, FAT12_FILENAME_FULL_BYTES + 1

.fat12_findDir_checkChar:

    dec cx
    jz .fat12_findDir_checkChar_end
    
    inc di
    lodsb

    cmp [di], al

    je .fat12_findDir_checkChar

.fat12_findDir_checkChar_end:

    popa
    
    ; check if succeeded 
    jz .fat12_findDir_entryFound

    ; check if first entry checked (started from end)
    cmp cx, 0
    jne .fat12_findDir_entry

.fat12_findDir_entryNotFound:

    jmp fat12_err_entryNotFound

.fat12_findDir_entryFound:
    
    ; Replace BX on stack
    mov di, sp
    add di, 8
    mov [di], bx

.fat12_findDir_end:

    popa
    ret

    ; ---------------------------------------
    ; Get file size from root dir entry
    ; 
    ; BX - address of dir entry
    ;
    ; Result - CX - file size in bytes
    ; ---------------------------------------

fat12_getSizeClusters:    
    
    pusha

    ; Get lower size bytes value
    add bx, FAT12_DIR_ENTRY_SIZE_BYTES_OFFSET
    mov ax, [bx]

    ; Get upper size bytes value
    add bx, 2
    mov dx, [bx]

    ; Make sure size not equal 0
    mov bx, ax
    or bx, dx
    jz fat12_err_sizeInvalid

    ; Convert to clusters
    mov cx, FAT12_CLUSTER_SIZE_BYTES
    div cx

    cmp dx, 0
    je .fat12_getSizeClusters_end

    inc ax

.fat12_getSizeClusters_end:

    ; Check size 
    jo fat12_err_sizeLimitExceeded
    cmp ax, FAT12_MAX_FILE_CLUSTERS
    jg fat12_err_sizeLimitExceeded

    ; Replace CX on stack
    mov di, sp
    add di, 12
    mov [di], ax

    popa
    ret
