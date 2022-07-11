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

    ; Calculate CHS
    push ax
    push bx
    push dx

    ; cylinder = LBA / (BPB_NUMBER_OF_HEADS * BPB_PHYSICAL_SECTORS_PER_TRACK)
	; temp = LBA % (BPB_NUMBER_OF_HEADS * BPB_PHYSICAL_SECTORS_PER_TRACK)
    mov ax, BPB_NUMBER_OF_HEADS
    mov bx, BPB_PHYSICAL_SECTORS_PER_TRACK
    mul bx

    push cx
    mov cx, ax
    pop ax

    xor dx, dx
    div cx

    ; Set cylinder (limited to 8 bits)
    mov cx, ax
    shl cx, 8

    ; head = temp / BPB_PHYSICAL_SECTORS_PER_TRACK
	; sector = temp % BPB_PHYSICAL_SECTORS_PER_TRACK + 1
    mov ax, dx
    xor dx, dx
    div bx
    
    ; Set sector
    mov cl, dl
    inc cl

    ; Set head
    pop dx
    mov dh, al

    pop bx
    pop ax

    ; Read (interrupt)
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
    call print_err_16
