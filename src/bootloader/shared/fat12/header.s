;
;  header.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 08.07.2022.
;

    ; ---------------------------------------
    ; Load FAT header from FAT12 disk
    ; 
    ; DL - disk number
    ; ---------------------------------------

fat12_loadHeader:

    pusha

    mov al, FAT12_HEADER_SECTORS
    mov bx, FAT_HEADER_ADDR
    mov cx, 1
    call io_read_disk

    popa
    ret
