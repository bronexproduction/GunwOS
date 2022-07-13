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
    ; Result - AX - address of dir entry
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

    popa
    
    ; check if succeeded 
    jz .fat12_findDir_entryFound

    ; check if first entry checked (started from end)
    cmp cx, 0
    jne .fat12_findDir_entry

.fat12_findDir_entryNotFound:

    mov bx, FAT12_LOADER_NOT_FOUND_STRING
    call print_err_16

.fat12_findDir_entryFound:
    
    ; Replace AX on stack
    mov di, sp
    add di, 14
    mov [di], bx

.fat12_findDir_end:

    popa
    ret

    ; ---------------------------------------
    ; Get file size from root dir entry
    ; 
    ; AX - address of dir entry
    ;
    ; Result - AX - file size in bytes
    ; ---------------------------------------

fat12_getSizeClusters:    
    
    pusha

    ; Make sure upper size bytes equal 0
    mov bx, ax
    add bx, FAT12_DIR_ENTRY_SIZE_BYTES_OFFSET + 2
    mov ax, [bx]

    cmp ax, 0
    jne .fat12_getSizeClusters_sizeInvalid

    ; Get lower size bytes value
    sub bx, 2
    mov ax, [bx]

    ; Make sure size not equal 0
    cmp ax, 0
    je .fat12_getSizeClusters_sizeInvalid

    ; Convert to clusters
    mov cx, FAT12_CLUSTER_SIZE_BYTES
    div cx

    cmp dx, 0
    je .fat12_getSizeClusters_end

    inc ax

.fat12_getSizeClusters_end:

    ; Replace AX on stack
    mov di, sp
    add di, 14
    mov [di], ax

    popa
    ret

.fat12_getSizeClusters_sizeInvalid:

    mov bx, FAT12_FILE_SIZE_INVALID
    call print_err_16
