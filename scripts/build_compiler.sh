#!/bin/bash

# STUB

set -e

TARGET=i386-elf
BUILD_DIR=compiler-build
SRC_DIR=$1

mkdir "$BUILD_DIR" && cd "$BUILD_DIR"
brew install gmp mpfr libmpc isl
$SRC_DIR/configure --target=$TARGET --disable-nls --enable-languages=c,c++ --without-headers
make
# make all-gcc
# make all-target-libgcc