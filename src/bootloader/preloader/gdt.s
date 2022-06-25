;
;  gdt.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 07.01.2018.
;

BITS 16

gdt_start:

gdt_null:
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xffff       ; Limit (0-15)
    dw 0x0          ; Base (0-15)
    db 0x0          ; Base (16-23)
    db 10011010b    ; Flags: (present)1 (privilege)00 (descriptor type)1 (code)1 (conforming)0 (readable)1 (accessed)0
    db 11001111b    ; Flags: (granularity)1 (32-bit default)1 (64-bit seg)0 (AVL)0, Limit (16-19)1111
    db 0x0          ; Base (24-31)

gdt_data:
    dw 0xffff       ; Limit (0-15)
    dw 0x0          ; Base (0-15)
    db 0x0          ; Base (16-23)
    db 10010010b    ; Flags: (present)1 (privilege)00 (descriptor type)1 (code)0 (expand down)0 (writable)1 (accessed)0
    db 11001111b    ; Flags: (granularity)1 (32-bit default)1 (64-bit seg)0 (AVL)0, Limit (16-19)1111
    db 0x0          ; Base (24-31)

gdt_end:

gdt_desc:
    dw gdt_end - gdt_start - 1  ; GDT size, always less one of the true size
    dd gdt_start                ; GDT start address

; GDT segment descriptor offsets
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
