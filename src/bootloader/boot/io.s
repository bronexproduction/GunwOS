;
;  io.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 18.01.2018.
;

BITS 16

; Read DH sectors to ES:BX from drive DL
io_read_disk:
    pusha
    push dx
    
    mov ah, 0x02    ; BIOS read sector function
    mov al, dh      ; number of sectors to read
    mov ch, 0x00    ; cylinder
    mov dh, 0x00    ; head
    
    int 0x13
    
    jc .io_read_disk_error
    
    pop dx
    cmp dh, al
    jne .io_read_disk_error
    
    popa
    ret

.io_read_disk_error:
    mov bx, IO_DISK_ERROR_MSG
    call print_str_16
    jmp $

IO_DISK_ERROR_MSG db "Disk read error!", 0
