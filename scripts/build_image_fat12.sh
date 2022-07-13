#!/bin/bash

set -e

BOOTLOADER_BYTES_EXPECTED=512
BOOTLOADER_SIGNATURE_EXPECTED=aa55
MEDIA_BYTES=1474560

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
	
