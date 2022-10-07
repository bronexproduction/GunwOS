;
;  data.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 03.01.2021.
;

BITS 16

BPB_BYTES_PER_LOGICAL_SECTOR        equ 512
BPB_LOGICAL_SECTORS_PER_CLUSTER     equ 1
BPB_NUMBER_OF_FATS                  equ 2
BPB_MAX_ROOT_DIRECTORY_ENTRIES      equ 224
BPB_TOTAL_LOGICAL_SECTORS           equ 2880
BPB_LOGICAL_SECTORS_PER_FAT         equ 9
BPB_PHYSICAL_SECTORS_PER_TRACK      equ 18
BPB_NUMBER_OF_HEADS                 equ 2

FAT_HEADER_ADDR                     equ 0x1000