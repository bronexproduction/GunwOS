;
;  fat12.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 06.07.2022.
;

BITS 16

FAT12_DIR_ENTRY_BYTES                   equ 32
FAT12_DIR_ENTRY_FIRST_CLUSTER_OFFSET    equ 26
FAT12_DIR_ENTRY_SIZE_BYTES_OFFSET       equ 28
FAT12_FILENAME_FULL_BYTES               equ 11
FAT12_CLUSTER_SIZE_BYTES                equ (BPB_BYTES_PER_LOGICAL_SECTOR * BPB_LOGICAL_SECTORS_PER_CLUSTER)
FAT12_DIR_ENTRIES_PER_SECTOR            equ (BPB_BYTES_PER_LOGICAL_SECTOR / FAT12_DIR_ENTRY_BYTES)
FAT12_ROOT_DIR_SECTORS                  equ (BPB_MAX_ROOT_DIRECTORY_ENTRIES / FAT12_DIR_ENTRIES_PER_SECTOR)
FAT12_ROOT_DIR_BYTES                    equ (FAT12_ROOT_DIR_SECTORS * BPB_BYTES_PER_LOGICAL_SECTOR)
FAT12_FATS_SECTORS                      equ (BPB_NUMBER_OF_FATS * BPB_LOGICAL_SECTORS_PER_FAT)
FAT12_FAT_BYTES                         equ (BPB_LOGICAL_SECTORS_PER_FAT * BPB_BYTES_PER_LOGICAL_SECTOR)
FAT12_FATS_BYTES                        equ (BPB_NUMBER_OF_FATS * FAT12_FAT_BYTES)
FAT12_HEADER_SECTORS                    equ (FAT12_FATS_SECTORS + FAT12_ROOT_DIR_SECTORS)
FAT12_MAX_READABLE_CLUSTER              equ (BPB_TOTAL_LOGICAL_SECTORS / BPB_LOGICAL_SECTORS_PER_CLUSTER - FAT12_ROOT_DIR_SECTORS - FAT12_FATS_SECTORS)

    ; ---------------------------------------
    ; Load file from FAT12 disk
    ; 
    ; AX - address of 11-bytes long filename
    ; DL - disk number
    ; SI - address of 16K unused memory
    ;      block to store FAT12 header
    ; DI - segment of read buffer (segment)
    ; ---------------------------------------

fat12_loadFile:

    pusha 

    ; Read FAT header sectors from the disk
    call fat12_loadHeader

    ; Verify FAT integrity
    call fat12_verifyFATs

    ; Find root directory entry for the file
    call fat12_findDir

    ; Now AX contains dir entry
    mov bx, ax

    ; Get size in clusters
    call fat12_getSizeClusters

    ; Now AX contains file size
    mov cx, ax

    ; Get first cluster
    add bx, FAT12_DIR_ENTRY_FIRST_CLUSTER_OFFSET
    mov ax, [bx]

    ; AX - First cluster
    ; CX - File size in clusters 

    ; Read file
    call fat12_readFile

    jmp $

    popa
    ret
    
    ; ---------------------------------------
    ; Load FAT header from FAT12 disk
    ; 
    ; SI - address of unused memory
    ;      block to store FAT12 header
    ;      (usually 16K for standard floppy)
    ; DL - disk number
    ; ---------------------------------------

fat12_loadHeader:

    pusha

    mov al, FAT12_HEADER_SECTORS
    mov bx, si
    mov cx, 2
    call io_read_disk

    popa
    ret

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

    ; ---------------------------------------
    ; Read file from FAT12 disk
    ; 
    ; AX - First cluster
    ; CX - File size in clusters
    ; DL - disk number
    ; SI - address of 16K unused memory
    ;      block to store FAT12 header
    ; DI - segment of read buffer (segment)
    ; ---------------------------------------

fat12_readFile:

    pusha

    mov es, di
    mov bx, 0

    ; AX - First cluster
    ; CX - File size in clusters 

.fat12_readFile_readCluster:    
    
    ; Validate
    call fat12_checkValidForRead

    ; Get next
    ; Validate next

    ; Read cluster

    dec cx
    mov ax, dx ; NEXT CLUSTER
    add bx, FAT12_CLUSTER_SIZE_BYTES

    cmp cx, 0
    jne .fat12_readFile_readCluster

    ; Fail if next cluster is not EOF

    jmp $

    mov di, 0
    mov es, di

    popa
    ret

    ; ---------------------------------------
    ; Check if sector valid for read
    ; 
    ; AX - sector num
    ; ---------------------------------------

fat12_checkValidForRead:

    pusha

    ; TODO: check if sector is valid for read
    cmp ax, 1
    jle .fat12_checkValidForRead_error

    cmp ax, FAT12_MAX_READABLE_CLUSTER
    jg .fat12_checkValidForRead_error

    popa
    ret

.fat12_checkValidForRead_error:

    mov bx, FAT12_INVALID_ERROR_STRING
    call print_err_16
    