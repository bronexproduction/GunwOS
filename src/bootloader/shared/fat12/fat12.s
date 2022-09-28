;
;  fat12.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 06.07.2022.
;

BITS 16

%include "../shared/fat12/data.s"
%include "../shared/fat12/header.s"
%include "../shared/fat12/fat.s"
%include "../shared/fat12/dir.s"
%include "../shared/fat12/read.s"
%include "../shared/fat12/check.s"

    ; ---------------------------------------
    ; Load file from FAT12 disk
    ; 
    ; AX - address of 11-bytes long filename
    ; DL - disk number
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

    ; Now BX contains dir entry

    ; Get size in clusters
    call fat12_getSizeClusters

    ; Now CX contains file size in clusters

    ; Check the size against the limit
    cmp cx, FAT12_READ_FILE_SIZE_LIMIT_CLUSTERS
    jg fat12_err_sizeLimitExceeded

    ; Get first cluster
    add bx, FAT12_DIR_ENTRY_FIRST_CLUSTER_OFFSET
    mov ax, [bx]

    ; AX - First cluster
    ; CX - File size in clusters 

    ; Read file
    call fat12_readFile

    popa
    ret
