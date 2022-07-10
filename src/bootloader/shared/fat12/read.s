;
;  read.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 08.07.2022.
;

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
    
    ; Validate current cluster
    call fat12_checkValidForRead
    jc .fat12_readFile_fatInvalid

    ; Get next cluster
    push ax
    call fat12_getEntry

    ; Validate next cluster
    call fat12_checkValidForRead
    jnc .fat12_readFile_readValidCluster
    call fat12_checkEOF
    jnc .fat12_readFile_readValidCluster
    jmp .fat12_readFile_fatInvalid

.fat12_readFile_readValidCluster:

    ; Read cluster
    ; SWAP AX WITH AX ON STACK (next cluster)
    ; CONFIGURE CORRECT SECTOR (offset)
    call io_read_disk

    ; Prepare next sector
    dec cx
    pop ax ; NEXT CLUSTER
    add bx, FAT12_CLUSTER_SIZE_BYTES

    ; Check if all sectors read
    cmp cx, 0
    jne .fat12_readFile_readCluster

    ; Fail if next cluster is not EOF
    call fat12_checkEOF
    jc .fat12_readFile_fatInvalid

    jmp $

    mov di, 0
    mov es, di

    popa
    ret

.fat12_readFile_fatInvalid:

    mov bx, FAT12_INVALID_ERROR_STRING
    call print_err_16
