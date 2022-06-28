;
;  data.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 03.01.2021.
;

KERNEL_TERMINATOR           equ 0xE5B1000B

BITS 16

KERNEL_BIN_OFFSET           equ BOOT_LOAD_SECTOR_BEGIN + BOOT_LOAD_SECTOR_ELF_OFFSET + BOOT_LOAD_SECTOR_COUNT - 1
KERNEL_MEM_OFFSET           equ 0x7E00
READ_BUFFER                 equ 0x0800
READ_BUFFER_SIZE            equ 512
%include "../shared/data.s"

BITS 32

PMODE_STACK                 equ 0x90000
A20_CHECK_OFFSET            equ 0x10000
