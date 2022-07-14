#!/bin/bash

set -e

BOOTLOADER_BYTES_EXPECTED=512
BOOTLOADER_SIGNATURE_EXPECTED=aa55
MEDIA_BYTES=1474560

BPB_4_0_BYTES_PER_SECTOR_OFFSET=0x00B
BPB_4_0_SECTORS_PER_CLUSTER_OFFSET=0x00D
BPB_4_0_NUMBER_OF_FATS_OFFSET=0x010
BPB_4_0_MAX_DIR_ENTRIES_OFFSET=0x011
BPB_4_0_TOTAL_LOGICAL_SECTORS_OFFSET=0x013
BPB_4_0_SECTORS_PER_FAT_OFFSET=0x016

FAT12_BYTES_PER_DIR_ENTRY=32

DESTINATION_IMAGE_FILE=$1
BOOTLOADER_FILE=$2

# Check bootloader existence
if [ ! -f "$BOOTLOADER_FILE" ]; then
    echo "Bootloader file not found: $BOOTLOADER_FILE"
    return 1
fi

# Check bootloader file size
BOOTLOADER_BYTES=$(wc -c "$BOOTLOADER_FILE" | awk '{print $1}')
if [ $BOOTLOADER_BYTES -ne $BOOTLOADER_BYTES_EXPECTED ]; then
    echo "Unexpected bootloader size of $BOOTLOADER_BYTES bytes"
    exit 1
fi

# Check bootloader signature
BOOTLOADER_SIGNATURE=$(tail -c 2 "$BOOTLOADER_FILE" | hexdump | awk '{print $2}')
if [[ "$BOOTLOADER_SIGNATURE" != "$BOOTLOADER_SIGNATURE_EXPECTED" ]]; then
    echo "Invalid bootloader signature: $BOOTLOADER_SIGNATURE"
    exit 1
fi

# Write bootloader
cat "$BOOTLOADER_FILE" > "$DESTINATION_IMAGE_FILE"

echo "Reading BPB..."

BYTES_PER_SECTOR=$(( 16#$(xxd -e -l2 -s $BPB_4_0_BYTES_PER_SECTOR_OFFSET "$BOOTLOADER_FILE" | awk '{print $2}') ))
SECTORS_PER_CLUSTER=$(( 16#$(xxd -e -l1 -s $BPB_4_0_SECTORS_PER_CLUSTER_OFFSET "$BOOTLOADER_FILE" | awk '{print $2}') ))
NUMBER_OF_FATS=$(( 16#$(xxd -e -l1 -s $BPB_4_0_NUMBER_OF_FATS_OFFSET "$BOOTLOADER_FILE" | awk '{print $2}') ))
SECTORS_PER_FAT=$(( 16#$(xxd -e -l2 -s $BPB_4_0_SECTORS_PER_FAT_OFFSET "$BOOTLOADER_FILE" | awk '{print $2}') ))
MAX_DIR_ENTRIES=$(( 16#$(xxd -e -l2 -s $BPB_4_0_MAX_DIR_ENTRIES_OFFSET "$BOOTLOADER_FILE" | awk '{print $2}') ))
TOTAL_LOGICAL_SECTORS=$(( 16#$(xxd -e -l2 -s $BPB_4_0_TOTAL_LOGICAL_SECTORS_OFFSET "$BOOTLOADER_FILE" | awk '{print $2}') ))

FAT_SIZE_BYTES=$(( BYTES_PER_SECTOR * SECTORS_PER_FAT ))
ROOT_DIR_BYTES=$(( MAX_DIR_ENTRIES * FAT12_BYTES_PER_DIR_ENTRY ))
HEADER_BYTES=$(( ROOT_DIR_BYTES + $(( NUMBER_OF_FATS * FAT_SIZE_BYTES )) ))

TOTAL_CLUSTERS=$(( TOTAL_LOGICAL_SECTORS / SECTORS_PER_CLUSTER ))
if [ $(expr $TOTAL_LOGICAL_SECTORS % $SECTORS_PER_CLUSTER) != "0" ]; then 
    echo "Total logical sectors do not align with sectors per cluster. Total logical sectors: $TOTAL_LOGICAL_SECTORS, Sectors per cluster: $SECTORS_PER_CLUSTER"
    exit 1
fi

HEADER_SECTORS=$(( HEADER_BYTES / BYTES_PER_SECTOR ))
if [ $(expr $HEADER_BYTES % $BYTES_PER_SECTOR) != "0" ]; then 
    echo "FAT header bytes do not align with sectors. Bytes: $HEADER_BYTES, Sector size: $BYTES_PER_SECTOR"
    exit 1
fi

HEADER_CLUSTERS=$(( HEADER_SECTORS / SECTORS_PER_CLUSTER ))
if [ $(expr $HEADER_SECTORS % $SECTORS_PER_CLUSTER) != "0" ]; then 
    echo "FAT header sectors do not align with clusters. Sectors: $HEADER_SECTORS, Cluster size (sectors): $SECTORS_PER_CLUSTER"
    exit 1
fi

MAX_CLUSTER_NUM=$(( $TOTAL_CLUSTERS - $HEADER_CLUSTERS ))

echo "Bytes per sector: $BYTES_PER_SECTOR"
echo "Cluster size (sectors): $SECTORS_PER_CLUSTER"
echo "Number of FATs: $NUMBER_OF_FATS"
echo "Sectors per FAT: $SECTORS_PER_FAT"
echo "Max dir entries: $MAX_DIR_ENTRIES"
echo "Total logical sectors: $TOTAL_LOGICAL_SECTORS"
echo "FAT size in bytes: $FAT_SIZE_BYTES"
echo "Root dir size in bytes: $ROOT_DIR_BYTES"
echo "Header size in bytes: $HEADER_BYTES"
echo "Header size in sectors: $HEADER_SECTORS"
echo "Header size in clusters: $HEADER_CLUSTERS"
echo "Max cluster number: $MAX_CLUSTER_NUM"

FAT_ENTRY_INDEX=2
DIR_ENTRY_INDEX=0
for filename in "${@:3}"; do
    if [ $DIR_ENTRY_INDEX -eq $MAX_DIR_ENTRIES ]; then
        echo "Max root directory entries count exceeded"
        exit 1
    fi



    DIR_ENTRY_INDEX=$((DIR_ENTRY_INDEX+1))
done
