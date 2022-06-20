#!/bin/bash

set -e

qemu-system-i386 -s -S -soundhw pcspk -drive file=build/system.img,index=0,if=floppy,format=raw
