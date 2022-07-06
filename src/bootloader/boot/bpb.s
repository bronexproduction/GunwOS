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
    ; BIOS parameter block byte limit
    ;
    ; The largest BPB size so far
    ; is used by FAT32 file system (79 bytes)
    ; so the limit is set
    ; to exceed that value just a bit
    ; --------------------------------------- 
BPB_BYTE_LIMIT                      equ 80

    ; ---------------------------------------
    ; FAT reserved sectors
    ; BIOS parameter block
    ; 
    ; Format: DOS 4.0 EBPB
    ; Media: 3.5-inch double sided
    ;        80 tracks per side
    ;        18 sectors per track
    ; --------------------------------------- 

BPB_BYTES_PER_LOGICAL_SECTOR        equ 512
BPB_NUMBER_OF_FATS                  equ 2
BPB_MAX_ROOT_DIRECTORY_ENTRIES      equ 224
BPB_LOGICAL_SECTORS_PER_FAT         equ 9

bpb_bytes_per_logical_sector:       dw BPB_BYTES_PER_LOGICAL_SECTOR
bpb_logical_sectors_per_cluster:    db 1
bpb_reserved_logical_sectors:       dw 1
bpb_number_of_fats:                 db BPB_NUMBER_OF_FATS
bpb_max_root_directory_entries:     dw BPB_MAX_ROOT_DIRECTORY_ENTRIES
bpb_total_logical_sectors:          dw 2880
bpb_media_descriptor:               db 0xF0
bpb_logical_sectors_per_fat:        dw BPB_LOGICAL_SECTORS_PER_FAT
bpb_physical_sectors_per_track:     dw 18
bpb_number_of_heads:                dw 2
bpb_hidden_sectors:                 dd 0
bpb_large_total_logical_sectors:    dd 0
bpb_physical_drive_number:          db 0
bpb_flags:                          db 0x0
bpb_extended_boot_signature:        db 0x29
bpb_volume_serial_number:           dd 1850637505
bpb_volume_label:                   db "GUNWOS     "
bpb_file_system_type:               db "FAT12   "

    ; --------------------------------------- 
    ; Bytes reserved
    ; for further BPB expansion
    ; up to BPB_BYTE_LIMIT
    ; --------------------------------------- 
bpb_safety_fill:                    times (BPB_START_OFFSET+BPB_BYTE_LIMIT)-($-$$) db 0