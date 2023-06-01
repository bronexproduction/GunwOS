#!/bin/bash

set -e
set -x

MODE=$2

if [[ "$MODE" == "debug"* ]]; then
    EPARAMS=("-s" "-S" "-d cpu_reset")
else
    EPARAMS=()
fi

qemu-system-i386 ${EPARAMS[@]} -monitor stdio -m 10M -drive file=$1,index=0,if=floppy,format=raw
