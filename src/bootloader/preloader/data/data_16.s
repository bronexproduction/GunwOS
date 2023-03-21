;
;  data_16.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 03.01.2021.
;

BITS 16

KERNEL_SEG          equ 0x1000
CLI_SEG             equ 0x5000

%include "../shared/data.s"
