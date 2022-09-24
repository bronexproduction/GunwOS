;
;  fat_space.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 15.09.2022.
;

fat_table_space:
; ---------------------------------------
;
;   Empty space dedicated
;   for FAT tables
;
;   Allows to use FAT12 code
;   from 1st stage bootloader
;   without messing with DS
;   (fat12_loadHeader loads FAT header
;   into ES:SI)
;
; ---------------------------------------

times FAT12_HEADER_BYTES db 0
