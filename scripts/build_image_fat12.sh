#!/bin/bash

set -e

BOOTLOADER_BYTES_EXPECTED=512
BOOTLOADER_SIGNATURE_EXPECTED=aa55
MEDIA_SIZE_BYTES=1474560

BPB_4_0_BYTES_PER_SECTOR_OFFSET=0x00B
BPB_4_0_SECTORS_PER_CLUSTER_OFFSET=0x00D
BPB_4_0_NUMBER_OF_FATS_OFFSET=0x010
BPB_4_0_MAX_DIR_ENTRIES_OFFSET=0x011
BPB_4_0_TOTAL_LOGICAL_SECTORS_OFFSET=0x013
BPB_4_0_SECTORS_PER_FAT_OFFSET=0x016

FAT12_BYTES_PER_DIR_ENTRY=32
FAT12_FILENAME_LENGTH=8
FAT12_EXTENSION_LENGTH=3
FAT12_ATTR_LENGTH=1
FAT12_FIRST_LOG_CLUSTER_LENGTH=2
FAT12_FILE_SIZE_BYTES_LENGTH=4

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

CLUSTER_SIZE_BYTES=$(( SECTORS_PER_CLUSTER * BYTES_PER_SECTOR ))
FAT_SIZE_BYTES=$(( BYTES_PER_SECTOR * SECTORS_PER_FAT ))
ROOT_DIR_SIZE_BYTES=$(( MAX_DIR_ENTRIES * FAT12_BYTES_PER_DIR_ENTRY ))
HEADER_BYTES=$(( ROOT_DIR_SIZE_BYTES + $(( NUMBER_OF_FATS * FAT_SIZE_BYTES )) ))

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
echo "Root dir size in bytes: $ROOT_DIR_SIZE_BYTES"
echo "Header size in bytes: $HEADER_BYTES"
echo "Header size in sectors: $HEADER_SECTORS"
echo "Header size in clusters: $HEADER_CLUSTERS"
echo "Max cluster number: $MAX_CLUSTER_NUM"

ROOT_DIR_DATA=()
FAT_DATA=(0x000 0x000)
FILES_DATA=()

FLAT_FILENAME_ARRAY=()

NEXT_CLUSTER=2

DIR_PADDING_LENGTH=$(( FAT12_BYTES_PER_DIR_ENTRY - FAT12_EXTENSION_LENGTH - FAT12_FILENAME_LENGTH - FAT12_ATTR_LENGTH - FAT12_FIRST_LOG_CLUSTER_LENGTH - FAT12_FILE_SIZE_BYTES_LENGTH ))
DIR_SEQ=($(seq 1 1 $DIR_PADDING_LENGTH))
DIR_PADDING=("${DIR_SEQ[@]/*/0x00}")
# Create FAT structure
echo "Creating FAT structure..."
for filename in "${@:3}"; do

    # Extract basename from path
    BASENAME=$(basename -- $filename)
    NAME=$(echo "$BASENAME" | awk -F. '{print $1}')
    EXTENSION=$(echo "$BASENAME" | awk -F. '{print $2}')

    # Check if basename contains ASCII characters only
    if [[ $BASENAME = *[![:ascii:]]* ]]; then
        echo "Filename can't contain non-ASCII characters ($BASENAME)"
        exit 1
    fi

    # Check if filename is 1-8 characters long
    if (( ${#NAME} < 1 || ${#NAME} > FAT12_FILENAME_LENGTH )); then
        echo "Filename length not in accepted range (1-8): $NAME ($BASENAME)"
        exit 1
    fi

    # Check if extension is 0-3 characters long
    if (( ${#EXTENSION} > FAT12_EXTENSION_LENGTH )); then
        echo "Extension length not in accepted range (0-3): $EXTENSION ($BASENAME)"
        exit 1
    fi

    # Create DOS filename with incorrect padding
    PADDING="........"
    DOS_FILENAME=$( printf "%s%s%s\n" "$NAME" "${PADDING:${#NAME}}" "$EXTENSION" )

    # Check if DOS_FILENAME already exists - exit if it does
    if [[ " ${FLAT_FILENAME_ARRAY[*]} " =~ " ${DOS_FILENAME} " ]]; then
        echo "Duplicated file: $BASENAME"
        exit 1
    fi
    
    # Prepare root dir entry

    # Check file existence
    if [ ! -f "$filename" ]; then
        echo "File $filename not found"
        exit 1
    fi

    # Get file size
    FILE_SIZE_BYTES=$(wc -c "$filename" | awk '{print $1}')
    if (( FILE_SIZE_BYTES < 1 )); then
        echo "File size invalid ($FILE_SIZE_BYTES). File: $filename"
        exit 1
    fi

    # Calculate file size in clusters
    FILE_SIZE_CLUSTERS=$(( FILE_SIZE_BYTES / CLUSTER_SIZE_BYTES ))
    if [ $(expr $FILE_SIZE_BYTES % $CLUSTER_SIZE_BYTES) != "0" ]; then 
        FILE_SIZE_CLUSTERS=$(( FILE_SIZE_CLUSTERS + 1 ))
    fi

    echo "File: $BASENAME"
    echo "* File size (bytes): $FILE_SIZE_BYTES"
    echo "* File size (clusters): $FILE_SIZE_CLUSTERS"
    echo "* Starting cluster: $NEXT_CLUSTER"
    
    # Create root directory entry
    DOS_FILENAME_ARR=$(echo "$DOS_FILENAME" | grep -o .)

    unset DIR
    DIR=(${DOS_FILENAME_ARR[@]})
    DIR+=(0x25) # Attributes: read-only, system, archive
    DIR+=(${DIR_PADDING[@]})
    DIR+=(
        $(printf '0x%.2x\n' $(( NEXT_CLUSTER % $((2 ** 8)) )))
        $(printf '0x%.2x\n' $(( NEXT_CLUSTER / $((2 ** 8)) ))) 
    ) # First logical cluster - 2 bytes
    DIR+=(
        $(printf '0x%.2x\n' $(( FILE_SIZE_BYTES % $((2 ** 8)) )))
        $(printf '0x%.2x\n' $(( FILE_SIZE_BYTES % $((2 ** 16)) / $((2 ** 8)) ))) 
        $(printf '0x%.2x\n' $(( FILE_SIZE_BYTES % $((2 ** 24)) / $((2 ** 16)) ))) 
        $(printf '0x%.2x\n' $(( FILE_SIZE_BYTES / $((2 ** 24)) ))) 
    ) # File size bytes - 4 bytes

    ROOT_DIR_DATA+=(${DIR[@]})

    # Append FAT entries
    if (( FILE_SIZE_CLUSTERS > 1 )); then
        for i in $(seq 1 1 $(( FILE_SIZE_CLUSTERS - 1 ))); do
            NEXT_CLUSTER=$(( NEXT_CLUSTER + 1))
            FAT_DATA+=($NEXT_CLUSTER)
        done
    fi
    NEXT_CLUSTER=$(( NEXT_CLUSTER + 1))
    FAT_DATA+=(0xFFF)

    FLAT_FILENAME_ARRAY+=($DOS_FILENAME)
done

# Check if ROOT_DIR_DATA within limits
if (( ${#ROOT_DIR_DATA[@]} > $ROOT_DIR_SIZE_BYTES )); then
    echo "Max root directory entries count/size in bytes exceeded"
    exit 1
fi

# Check if FAT data within limits
if [ $(expr ${#FAT_DATA[@]} % 2) != "0" ]; then
    FAT_DATA+=(0x000)
fi
if (( ${#FAT_DATA[@]} > $(( MAX_CLUSTER_NUM + 1 )) )); then
    echo "FAT cluster limit exceeded: ${#FAT_DATA[@]}"
    exit 1
fi

echo "FAT:"
echo ${FAT_DATA[@]}
echo "Root dir: (. to be replaced with spaces)"
echo ${ROOT_DIR_DATA[@]}

FAT_BYTES=()

echo "Converting..."
# Convert FAT
while [ ${#FAT_DATA[@]} -gt 0 ]; do

    ENTRY0=${FAT_DATA[0]}
    if [[ $ENTRY0 == 0x* ]]; then
        ENTRY0=$(( 16#${ENTRY0:2} ))
    fi
    ENTRY1=${FAT_DATA[1]}
    if [[ $ENTRY1 == 0x* ]]; then
        ENTRY1=$(( 16#${ENTRY1:2} ))
    fi

    BYTE0="$(( ENTRY0 & 0x0FF ))"
    BYTE1="$(( $(( $(( ENTRY0 & 0xF00 )) >> 8 )) | $(( $(( ENTRY1 & 0x00F )) << 4 )) ))"
    BYTE2="$(( $(( ENTRY1 & 0xFF0 )) >> 4 ))"

    FAT_DATA=("${FAT_DATA[@]:2}")
    FAT_BYTES+=($BYTE0 $BYTE1 $BYTE2)
done

if (( ${#FAT_BYTES[@]} > FAT_SIZE_BYTES )); then
    echo "FAT size in bytes exceeded: ${#FAT_BYTES[@]}"
    exit 1
fi

echo "FAT bytes:"
echo ${FAT_BYTES[@]}

echo "Writing FAT tables..."
# Write FATs
for i in $(seq 1 $NUMBER_OF_FATS); do
    for byte in ${FAT_BYTES[@]}; do
        printf "\x$(printf %x $byte)" >> "$DESTINATION_IMAGE_FILE"
    done
    
    # Expand destination file to fill remaining FAT bytes
    truncate -s $(( i * FAT_SIZE_BYTES + BOOTLOADER_BYTES_EXPECTED )) "$DESTINATION_IMAGE_FILE"
done

echo "Writing root directory..."
# Write root dir
for element in ${ROOT_DIR_DATA[@]}; do
    if [[ "$element" == "." ]]; then
        BYTE=$( echo " " | od -An -vtu1 | awk '{print $1}' )
    elif [[ "$element" == 0x* ]]; then
        BYTE=$(( 16#${element:2} ))
    else
        BYTE=$( printf '%s\n' "$element" | awk '{ print toupper($0) }' | od -An -vtu1 | awk '{print $1}' )
    fi
    printf "\x$(printf %x $BYTE)" >> "$DESTINATION_IMAGE_FILE"
done

# Expand destination file to fill remaining root dir table bytes
truncate -s $(( NUMBER_OF_FATS * FAT_SIZE_BYTES + ROOT_DIR_SIZE_BYTES + BOOTLOADER_BYTES_EXPECTED )) "$DESTINATION_IMAGE_FILE"

echo "Writing files..."
# Write files    
for filename in "${@:3}"; do
    cat "$filename" >> "$DESTINATION_IMAGE_FILE"

    # Expand destination file to fill remaining root dir table bytes (align to clusters) if needed
    FILE_SIZE_BYTES=$(wc -c "$DESTINATION_IMAGE_FILE" | awk '{print $1}')
    if [ $(expr $FILE_SIZE_BYTES % $CLUSTER_SIZE_BYTES) != "0" ]; then
        truncate -s $(( FILE_SIZE_BYTES / CLUSTER_SIZE_BYTES * CLUSTER_SIZE_BYTES + CLUSTER_SIZE_BYTES )) "$DESTINATION_IMAGE_FILE"
    fi
done

# Check if image size fits the media
FILE_SIZE_BYTES=$(wc -c "$DESTINATION_IMAGE_FILE" | awk '{print $1}')
if (( FILE_SIZE_BYTES > MEDIA_SIZE_BYTES )); then
    echo "Image size ($FILE_SIZE_BYTES) exceeds media size ($MEDIA_SIZE_BYTES)"
    exit 1
fi

echo "Image size: $FILE_SIZE_BYTES bytes"
