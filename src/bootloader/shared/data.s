;
;  data.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 03.01.2021.
;

BITS 16

BOOT_LOAD_SECTOR_BEGIN      equ 0x02
BOOT_LOAD_SECTOR_ELF_OFFSET equ 0x08
BOOT_LOAD_SECTOR_COUNT      equ 5
