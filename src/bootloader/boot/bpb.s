;
;  bpb.s
;
;  BIOS Parameter Block
;
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 28.06.2022.
;

BITS 16

    ; --------------------------------------- 
    ; BIOS parameter block start offset
    ; --------------------------------------- 
BPB_START_OFFSET                    equ 0xB

    ; ---------------------------------------
    ; FAT reserved sectors
    ; BIOS parameter block
    ; 
    ; Format: DOS 4.0 EBPB (51 bytes)
    ; Media: 3.5-inch double sided
    ;        80 tracks per side
    ;        18 sectors per track
    ; --------------------------------------- 

BPB_BYTES_PER_LOGICAL_SECTOR        equ 512
BPB_LOGICAL_SECTORS_PER_CLUSTER     equ 1
BPB_NUMBER_OF_FATS                  equ 2
BPB_MAX_ROOT_DIRECTORY_ENTRIES      equ 224
BPB_TOTAL_LOGICAL_SECTORS           equ 2880
BPB_LOGICAL_SECTORS_PER_FAT         equ 9
BPB_PHYSICAL_SECTORS_PER_TRACK      equ 18
BPB_NUMBER_OF_HEADS                 equ 2

bpb_bytes_per_logical_sector:       dw BPB_BYTES_PER_LOGICAL_SECTOR
bpb_logical_sectors_per_cluster:    db BPB_LOGICAL_SECTORS_PER_CLUSTER
bpb_reserved_logical_sectors:       dw 1
bpb_number_of_fats:                 db BPB_NUMBER_OF_FATS
bpb_max_root_directory_entries:     dw BPB_MAX_ROOT_DIRECTORY_ENTRIES
bpb_total_logical_sectors:          dw BPB_TOTAL_LOGICAL_SECTORS
bpb_media_descriptor:               db 0xF0
bpb_logical_sectors_per_fat:        dw BPB_LOGICAL_SECTORS_PER_FAT
bpb_physical_sectors_per_track:     dw BPB_PHYSICAL_SECTORS_PER_TRACK
bpb_number_of_heads:                dw BPB_NUMBER_OF_HEADS
; bpb_hidden_sectors:                 dd 0
; bpb_large_total_logical_sectors:    dd 0
; bpb_physical_drive_number:          db 0
; bpb_flags:                          db 0x0
; bpb_extended_boot_signature:        db 0x29
; bpb_volume_serial_number:           dd 1850637505
;bpb_volume_label:                   db "GUNWOS     "
;bpb_file_system_type:               db "FAT12   "
