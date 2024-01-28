#!/bin/bash

set -e
set -x

MODE=$2
TEST_FILE=$3
DEBUG_FLAGS=("-s" "-S" "-d cpu_reset")
UNIT_TEST_FLAGS=("-device" "isa-debug-exit,iobase=0xf4,iosize=0x01" "-device" "loader,file=$TEST_FILE,addr=0x00400000,force-raw=on")

# needs some improvements

# qemu exit on cpu reset during testing

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
