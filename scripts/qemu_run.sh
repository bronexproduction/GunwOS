#!/bin/bash

set -e
set -x

MODE=$2
DEBUG_FLAGS=("-s" "-S" "-d cpu_reset")
UNIT_TEST_FLAGS=("-device" "isa-debug-exit,iobase=0xf4,iosize=0x01")

# needs some improvements

if [[ "$MODE" == "debug"* ]]; then
    EPARAMS=("-serial stdio" ${DEBUG_FLAGS[@]})
elif [[ "$MODE" == "test_unit_debug"* ]]; then
    EPARAMS=("-serial stdio" ${DEBUG_FLAGS[@]} ${UNIT_TEST_FLAGS[@]})
elif [[ "$MODE" == "test_unit"* ]]; then
    EPARAMS=("-serial stdio" ${UNIT_TEST_FLAGS[@]})
else
    EPARAMS=("-monitor stdio")
fi

qemu-system-i386 ${EPARAMS[@]} -m 10M -drive file=$1,index=0,if=floppy,format=raw
