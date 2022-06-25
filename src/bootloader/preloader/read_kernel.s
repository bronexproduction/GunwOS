;
;  read_kernel.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 04.01.2021.
;

BITS 16

; Read kernel data

read_kernel:
    pusha

    call io_read_drive_parameters                   ; read drive parameters
    
    mov bx, MSG_BOOT_DRIVE              
    call print_str_16

    mov bx, MSG_BOOT_DRIVE_INDEX
    call print_str_16
    mov al, [io_data_drive_index]
    call print_hex_byte
    mov bx, IO_NEWLINE
    call print_str_16

    mov bx, MSG_BOOT_DRIVE_SECTORS_PER_TRACK
    call print_str_16
    mov al, [io_data_drive_sectors_per_track]
    call print_hex_byte
    mov bx, IO_NEWLINE
    call print_str_16

    mov bx, MSG_BOOT_DRIVE_CYLINDERS
    call print_str_16
    mov ax, [io_data_drive_cylinders]
    call print_hex_word
    mov bx, IO_NEWLINE
    call print_str_16

    mov bx, MSG_BOOT_DRIVE_HEADS
    call print_str_16
    mov al, [io_data_drive_heads]
    call print_hex_byte
    mov bx, IO_NEWLINE
    call print_str_16

    mov ax, KERNEL_MEM_OFFSET                       ; destination memory address - kernel start
    mov bx, READ_BUFFER                             ; set read buffer memory address
    mov cx, 0x0000                                  ; set initial destination offset
    mov dx, READ_BUFFER_SIZE                        ; byte count
    mov [io_data_sector], byte KERNEL_BIN_OFFSET    ; set initial loading sector value

read_kernel_sector:
    call io_read_sect                               ; read sector

    pusha
    mov eax, KERNEL_TERMINATOR                      ; if sector filled with KERNEL_TERMINATOR - jmp end    
    mov bx, READ_BUFFER
    mov cx, READ_BUFFER_SIZE
    call mem_verify_4b_pattern
    jc read_kernel_pattern_err                      ; if error - halt
    cmp al, 0x00
    jz read_kernel_pattern_zero                     ; if terminator pattern found - exiting

read_kernel_sector_pattern_not_matching:
    popa
    jmp read_kernel_copy_buffer

read_kernel_pattern_zero:
    popa
    jmp read_kernel_success

read_kernel_copy_buffer:    
    call mem_copy                                   ; copy current sector to the destination memory address

read_kernel_offset_increment:
    add cx, 0x0020                                  ; offset destination by 512 bytes
    
read_kernel_chs_increment:    
    push ax
    
    mov al, [io_data_sector]                        ; increment sector
    inc al
    mov [io_data_sector], al
    cmp al, [io_data_drive_sectors_per_track]                          
    jle read_kernel_chs_increment_end               ; if sector value less or equal io_data_drive_sectors_per_track - finish incrementing

    mov [io_data_sector], byte 0x01                 ; else set sector to 1
    mov al, [io_data_head]                          ; and increment head
    inc al
    mov [io_data_head], al
    cmp al, [io_data_drive_heads]
    jl read_kernel_chs_increment_end                ; if head value lower than io_data_drive_heads - finish incrementing    

    mov [io_data_head], byte 0x00                   ; else set head to 0
    mov ax, [io_data_cylinder]                      ; and increment cylinder
    inc ax
    mov [io_data_cylinder], ax          
    cmp ax, [io_data_drive_cylinders]
    jl read_kernel_chs_increment_end                ; if cylinder value less than io_data_drive_cylinders - finish incrementing

    jmp read_kernel_end_of_drive_err                ; else reached end of the drive - error

read_kernel_chs_increment_end:
    pop ax
    jmp read_kernel_sector                          ; jmp read sector

read_kernel_pattern_err:
    mov bx, MSG_KERNEL_PATTERN_ERR
    call print_str_16
    jmp $

read_kernel_end_of_drive_err:
    mov bx, MSG_KERNEL_END_OF_DRIVE_ERR
    call print_str_16
    jmp $

read_kernel_success:
    mov bx, MSG_KERNEL_LOAD_SUCCESS
    call print_str_16

read_kernel_end:
    popa
    ret
