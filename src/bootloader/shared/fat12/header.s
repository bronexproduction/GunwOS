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
    ; SI - address of unused memory
    ;      block to store FAT12 header
    ;      (usually 16K for standard floppy)
    ; DL - disk number
    ; ---------------------------------------

fat12_loadHeader:

    pusha

    mov al, FAT12_HEADER_SECTORS
    mov bx, si
    mov cx, 1
    call io_read_disk

    popa
    ret
