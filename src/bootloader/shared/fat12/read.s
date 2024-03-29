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
    ; DI - segment of read buffer (segment)
    ; ---------------------------------------

fat12_readFile:

    pusha
    push es

    mov es, di
    xor bx, bx

    ; AX - First cluster
    ; CX - File size in clusters 

.fat12_readFile_readCluster:    

    ; Validate current cluster
    call fat12_checkValidForRead
    jc fat12_err_clusterNotValidForRead

    ; Get next cluster
    push ax
    call fat12_getEntry

    ; Validate next cluster
    call fat12_checkValidForRead
    jnc .fat12_readFile_readValidCluster
    call fat12_checkEOF
    jnc .fat12_readFile_readValidCluster
    jmp fat12_err_fatEntryInvalid

.fat12_readFile_readValidCluster:

    ; Read cluster

    ; Swap AX with AX on stack (next cluster should stay on stack)
    push bx
    mov bx, ax
    add sp, 2
    pop ax
    push bx
    sub sp, 2 
    pop bx

    ; Read sector
    pusha
    mov cx, ax
    add cx, FAT12_DATA_CLUSTER_OFFSET
    mov al, BPB_LOGICAL_SECTORS_PER_CLUSTER
    call io_read_disk
    popa

    ; Prepare next sector
    dec cx
    pop ax ; Next cluster
    add bx, FAT12_CLUSTER_SIZE_BYTES
    jnc .fat12_readFile_checkEOF

    ; Handle address overflow
    ; (changing segment)
    push bx
    mov bx, es
    add bx, 0x1000
    mov es, bx
    pop bx

.fat12_readFile_checkEOF:    

    ; Check if all sectors read
    cmp cx, 0
    jne .fat12_readFile_readCluster

    ; Fail if next cluster is not EOF
    call fat12_checkEOF
    jc fat12_err_fatEntryInvalidEOFExpected

    pop es
    popa
    ret
