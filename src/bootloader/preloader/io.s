;
;  io.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 18.01.2018.
;

BITS 16

; Drive data

io_data_drive_index:
    db 0x00
io_data_drive_sectors_per_track:
    db 0x00
io_data_drive_cylinders:
    dw 0x0000
io_data_drive_heads:
    db 0x00

; Current CHS address
;
; used by io_read_disk

io_data_sector:
    db 0x00
io_data_cylinder:
    dw 0x0000
io_data_head:
    db 0x00

; Read drive parameters
;
; Performs 13h BIOS interrupt with function 08h
; to determine io_data_drive configuration
; Fills the io_data_drive_* fields

io_read_drive_parameters:
    pusha
    push es                                     ; workaround?
    push di                                     ; workaround?

    xor ax, ax                          
    mov es, ax                                  ; workaround?
    xor di, ax                                  ; workaround?

    mov ah, 0x08
    mov dl, [io_data_drive_index]

    int 0x13

    jc io_read_drive_parameters_error           ; on error - jump to handler

    inc dh                                      ; DH contains logical last index of heads (always 1 less than its count)
    mov [io_data_drive_heads], dh

    mov ax, cx
    and ax, 0x00C0                              ; bits 7-6 in CL contains two MSB of cylinder value
    shl ax, 2
    mov al, ch                                  ; CH contains 8 LSB of cylinder value
    inc ax                                      ; CX contains logical last index of cylinders (always 1 less than its count)
    mov [io_data_drive_cylinders], ax

    mov al, cl                                  ; only bits 5-0 of CL register contain information about sectors
    and al, 0x3F                                ; CL (bits 5-0) contains logical last index of sector BUT sector count is equal to the index (sector index starts at 1 instead of 0)                            
    mov [io_data_drive_sectors_per_track], al

io_read_drive_parameters_end:
    pop di
    pop es
    popa
    ret

io_read_drive_parameters_error:
    mov bx, IO_DISK_READ_PARAMETER_ERROR_MSG
    call print_str_16
    
    mov al, ah                                  ; print error code
    call print_hex_byte
    jmp $

; Read DH sectors to ES:BX from drive DL
;
; Params:
; BX - destination memory address
; Drive and CHS config taken from io_data_* (see above)

io_read_disk:
    pusha

    mov al, [io_data_drive_sectors_per_track]   ; check drive configuration
    cmp al, 0
    je .io_read_disk_no_drive_data_error
    mov ax, [io_data_drive_cylinders]
    cmp ax, 0
    je .io_read_disk_no_drive_data_error
    mov al, [io_data_drive_heads]
    cmp al, 0
    je .io_read_disk_no_drive_data_error

    mov ah, 0x02                    ; BIOS read sector function
    mov al, 0x01                    ; number of sectors to read

    mov dx, [io_data_cylinder]      ; cylinder            
    mov cl, dh                      ; CL bits (7-6) - upper bits of cylinder number
    shl cl, 6
    mov ch, dl                      ; CH - lower bits of cylinder number

    mov dl, [io_data_sector]        ; sector
    and dl, 0x3F                    ; CL bits (5-0) - sector number
    or cl, dl                       
   
    mov dh, [io_data_head]          ; head
    mov dl, [io_data_drive_index]   ; drive
    
    int 0x13
    
    jc .io_read_disk_error          ; if error happened - jump to error handler

    cmp al, 0x01                    ; if sectors read count incorrect - jump to error handler
    jne .io_read_disk_sector_count_error 
    
    popa
    ret

.io_read_disk_error:
    mov bx, IO_DISK_ERROR_MSG
    call print_str_16
    
    mov al, ah                      ; print error code
    call print_hex_byte

    mov bx, IO_DISK_READ_SECT       ; print sector
    call print_str_16
    mov al, [io_data_sector]
    call print_hex_byte

    mov bx, IO_DISK_READ_CYLINDER   ; print cylinder
    call print_str_16
    mov ax, [io_data_cylinder]
    call print_hex_word

    mov bx, IO_DISK_READ_HEAD       ; print head
    call print_str_16
    mov al, [io_data_head]
    call print_hex_byte
    
    mov bx, IO_DISK_READ_DRIVE      ; print drive
    call print_str_16
    mov al, [io_data_drive_index]
    call print_hex_byte

    jmp $

.io_read_disk_no_drive_data_error:
    mov bx, IO_DISK_SECTOR_NO_DRIVE_DATA_ERROR_MSG
    call print_str_16
    jmp $

.io_read_disk_sector_count_error:
    mov bx, IO_DISK_SECTOR_COUNT_ERROR_MSG
    call print_str_16
    jmp $

IO_DISK_ERROR_MSG db 'Disk read error! Code: ', 0
IO_DISK_READ_PARAMETER_ERROR_MSG db 'Read drive parameters error! Code: ', 0
IO_DISK_SECTOR_COUNT_ERROR_MSG db 'Disk read sector count mismatch!', 0
IO_DISK_SECTOR_NO_DRIVE_DATA_ERROR_MSG db 'Drive data not set!', 0
IO_DISK_READ_SECT db ' at sect: ', 0
IO_DISK_READ_CYLINDER db ', cyl: ', 0
IO_DISK_READ_HEAD db ', head: ', 0
IO_DISK_READ_DRIVE db ', drive: ', 0
IO_NEWLINE db 0xa, 0xd, 0
