;
;  kernel_data.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 03.01.2021.
;

E820_MAX_ENTRIES    equ 128
E820_ENTRY_BYTES    equ 24

kernel_data:
CONV_MEM_AVAILABLE                          dw 0
kernel_data_e820_entries:
times E820_MAX_ENTRIES*E820_ENTRY_BYTES     db 0
