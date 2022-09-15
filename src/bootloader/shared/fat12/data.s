;
;  fat12_data.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 08.07.2022.
;

FAT12_DIR_ENTRY_BYTES                   equ 32
FAT12_DIR_ENTRY_FIRST_CLUSTER_OFFSET    equ 26
FAT12_DIR_ENTRY_SIZE_BYTES_OFFSET       equ 28
FAT12_FILENAME_FULL_BYTES               equ 11
FAT12_CLUSTER_SIZE_BYTES                equ (BPB_BYTES_PER_LOGICAL_SECTOR * BPB_LOGICAL_SECTORS_PER_CLUSTER)
FAT12_DIR_ENTRIES_PER_SECTOR            equ (BPB_BYTES_PER_LOGICAL_SECTOR / FAT12_DIR_ENTRY_BYTES)
FAT12_ROOT_DIR_SECTORS                  equ (BPB_MAX_ROOT_DIRECTORY_ENTRIES / FAT12_DIR_ENTRIES_PER_SECTOR)
FAT12_ROOT_DIR_BYTES                    equ (FAT12_ROOT_DIR_SECTORS * BPB_BYTES_PER_LOGICAL_SECTOR)
FAT12_FATS_SECTORS                      equ (BPB_NUMBER_OF_FATS * BPB_LOGICAL_SECTORS_PER_FAT)
FAT12_FAT_BYTES                         equ (BPB_LOGICAL_SECTORS_PER_FAT * BPB_BYTES_PER_LOGICAL_SECTOR)
FAT12_FATS_BYTES                        equ (BPB_NUMBER_OF_FATS * FAT12_FAT_BYTES)
FAT12_HEADER_SECTORS                    equ (FAT12_FATS_SECTORS + FAT12_ROOT_DIR_SECTORS)
FAT12_HEADER_BYTES                      equ (FAT12_HEADER_SECTORS + BPB_BYTES_PER_LOGICAL_SECTOR)
FAT12_MAX_READABLE_CLUSTER              equ (BPB_TOTAL_LOGICAL_SECTORS / BPB_LOGICAL_SECTORS_PER_CLUSTER - FAT12_ROOT_DIR_SECTORS - FAT12_FATS_SECTORS)
FAT12_LBA_CYLINDER_DIVISOR              equ (BPB_NUMBER_OF_HEADS * BPB_PHYSICAL_SECTORS_PER_TRACK)
FAT12_DATA_CLUSTER_OFFSET               equ (FAT12_HEADER_SECTORS - 1)
