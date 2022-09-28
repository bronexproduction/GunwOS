;
;  data_16.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 03.01.2021.
;

BITS 16

KERNEL_SEG          equ 0x2000
%include "../shared/data.s"
%include "../shared/fat12/data.s"
