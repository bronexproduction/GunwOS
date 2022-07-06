;
;  io.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 18.01.2018.
;

BITS 16

    ; ---------------------------------------
    ; Read sectors from disk
    ; 
    ; AL - sector count
    ; ES:BX - destination address
    ; CX - starting index (LBA)
    ; DL - disk number
    ; ---------------------------------------

io_read_disk:
    pusha
    xor ah, ah
    push ax
    
    ; --------------------------------------- 
    ; FDC reset procedure
    ; --------------------------------------- 
    mov ah, 0x00
    
    int 0x13
    jc .io_read_disk_error

    ; --------------------------------------- 
    ; BIOS read function
    ; 
    ; AH - 0x02
    ; AL - Sectors to read count
    ; ES:BX - Buffer address
    ; CH - Cylinder
    ; CL - Sector
    ; DH - Head
    ; DL - Drive
    ; --------------------------------------- 
    mov ah, 0x02    ; BIOS read sector function

    ; PREPARE STARTING INDEX

    mov ch, 0x00    ; cylinder
    mov dh, 0x00    ; head
    
    int 0x13
    jc .io_read_disk_error

    xor dx, dx
    mov dl, al
    pop ax
    cmp dl, al
    jne .io_read_disk_error
    
    popa
    ret

.io_read_disk_error:
    mov bx, IO_DISK_ERROR_MSG
    call print_str_16
    jmp $

IO_DISK_ERROR_MSG db "Disk read error!", 0
