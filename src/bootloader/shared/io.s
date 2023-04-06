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

.io_read_disk_calculate_chs:

    ; Calculate CHS
    push ax
    push bx
    push dx

    ; ax = LBA
    ; bx = BPB_NUMBER_OF_HEADS * BPB_PHYSICAL_SECTORS_PER_TRACK
    ; cylinder = LBA / (BPB_NUMBER_OF_HEADS * BPB_PHYSICAL_SECTORS_PER_TRACK)
    mov ax, cx
    mov bx, FAT12_LBA_CYLINDER_DIVISOR
    xor dx, dx
    div bx

    ; Set cylinder (limited to 8 bits)
    mov ch, al

    ; head = temp / BPB_PHYSICAL_SECTORS_PER_TRACK
	; sector = temp % BPB_PHYSICAL_SECTORS_PER_TRACK + 1
    mov ax, dx
    mov bx, BPB_PHYSICAL_SECTORS_PER_TRACK
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

.io_read_disk_read_chs:

    ; Read (interrupt)
    int 0x13
    jc io_read_sector_failed

    pop dx
    cmp dl, al
    jne io_sector_count_mismatch
    
    popa
    ret
