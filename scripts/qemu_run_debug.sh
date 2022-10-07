#!/bin/bash

set -e

qemu-system-i386 -monitor stdio -s -S -soundhw pcspk -drive file=$1,index=0,if=floppy,format=raw
