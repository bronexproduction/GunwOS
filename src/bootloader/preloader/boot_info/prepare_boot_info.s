;
;  prepare_boot_info.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 08.05.2024.
;

BITS 16

%include "boot_info/memory.s"

prepare_boot_info:
    call detect_lower_memory
    call detect_upper_memory
    ret