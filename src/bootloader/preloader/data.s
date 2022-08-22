;
;  data.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 03.01.2021.
;

BITS 16

KERNEL_SEG          equ 0x2000
%include "../shared/data.s"

BITS 32

PMODE_STACK         equ 0x90000
A20_CHECK_OFFSET    equ 0x10000
