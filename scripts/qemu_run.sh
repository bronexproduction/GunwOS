#!/bin/bash

set -e
set -x

MODE=$2

if [[ "$MODE" == "debug"* ]]; then
    EPARAMS=("-s" "-S")
else
    EPARAMS=()
fi

qemu-system-i386 ${EPARAMS[@]} -monitor stdio -drive file=$1,index=0,if=floppy,format=raw
