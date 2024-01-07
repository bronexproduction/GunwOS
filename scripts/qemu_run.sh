#!/bin/bash

set -e
set -x

MODE=$2

if [[ "$MODE" == "debug"* ]]; then
    EPARAMS=("-serial stdio" "-s" "-S" "-d cpu_reset")
elif [[ "$MODE" == "test_unit"* ]]; then
    EPARAMS=("-serial stdio" "-s" "-S")
else
    EPARAMS=("-monitor stdio")
fi

qemu-system-i386 ${EPARAMS[@]} -m 10M -drive file=$1,index=0,if=floppy,format=raw
