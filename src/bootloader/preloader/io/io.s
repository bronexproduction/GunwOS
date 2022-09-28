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
    mov bx, MSG_IO_DISK_READ_PARAMETER_ERROR_MSG
    call print_str_16
    
    mov al, ah                                  ; print error code
    call print_hex_byte
    cli
    hlt

%include "../shared/io.s"
