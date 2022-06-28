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
    ; Format: DOS 2.0
    ; Media: 3.5-inch double sided
    ;        80 tracks per side
    ;        18 sectors per track
    ; --------------------------------------- 
bpb_bytes_per_logical_sector:       dw 512
bpb_logical_sectors_per_cluster:    db 1
bpb_reserved_logical_sectors:       dw 1
bpb_number_of_fats:                 db 2
bpb_root_directory_entries:         dw 224
bpb_total_logical_sectors:          dw 2880
bpb_media_descriptor:               db 0xF0
bpb_logical_sectors_per_fat:        dw 9

    ; --------------------------------------- 
    ; Bytes reserved
    ; for further BPB expansion
    ; up to BPB_BYTE_LIMIT
    ; --------------------------------------- 
bpb_safety_fill:                    times (BPB_START_OFFSET+BPB_BYTE_LIMIT)-($-$$) db 0