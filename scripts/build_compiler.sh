#!/bin/bash

WORKSPACE_DIR="gcc-workspace"
BUILD_DIR="gcc-build"
MIRROR="https://ftp.mpi-inf.mpg.de/mirrors/gnu/mirror/gcc.gnu.org/pub/gcc/releases"
VERSION=$1
CHECKSUM_FILE="sha512.sum"
INSTALL_DIR="$PWD/gunwxcc-$VERSION"
TARGET=i386-elf

set -e

# Create directory structure
rm -rf "$WORKSPACE_DIR"
mkdir "$WORKSPACE_DIR"
cd "$WORKSPACE_DIR"

# Download sources
wget "$MIRROR/gcc-$VERSION/gcc-$VERSION.tar.gz"
wget "$MIRROR/gcc-$VERSION/$CHECKSUM_FILE"

# Verify checksum
CHECKSUM="$(echo $(shasum -a512 gcc-$VERSION.tar.gz) | awk '{print $1}')"
EXPECTED_CHECKSUM="$(cat sha512.sum | grep gcc-$VERSION.tar.gz$ | awk '{print $1}')"

if [ $CHECKSUM != $EXPECTED_CHECKSUM ]; then 
    echo "Invalid file checksum"
    exit 1
fi
  
# Unpack sources
tar -xvzf "gcc-$VERSION.tar.gz"

# Create build directory
mkdir build-gcc
cd build-gcc

# Install dependencies
sudo apt install "$DEPENDENCIES"

# Build
bash "../gcc-$VERSION/configure" "--prefix=$INSTALL_DIR" "--target=$TARGET"  --enable-languages=c,c++ --disable-multilib --disable-nls --without-headers
make -j 4

# Install
mkdir $INSTALL_DIR
make install
