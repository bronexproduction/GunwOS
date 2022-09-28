;
;  preloader.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 03.01.2021.
;

ORG 0x8000

BITS 16

    ; ---------------------------------------
    ; 16-bit Real Mode fragment
    ; ---------------------------------------

%include "preloader_16.s"

    ; ---------------------------------------
    ; 32-bit Protected Mode fragment
    ; ---------------------------------------

BITS 32

%include "preloader_32.s"

    ; ---------------------------------------
    ; Halt the CPU
    ;
    ; NOTE:
    ; Just in case kernel entry
    ; fails for some reason
    ; ---------------------------------------

    cli
    hlt

%include "data/msg.s"
