;
;  data_32.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 28.09.2022.
;

BITS 32

KERNEL_ADDR         equ (KERNEL_SEG << 4)

PMODE_STACK         equ 0x00300000
A20_CHECK_OFFSET    equ 0x10000
