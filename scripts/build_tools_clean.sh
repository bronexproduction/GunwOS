#!/bin/bash

TEMP_DIR="$PWD/temp"
WORKSPACE_DIR="$TEMP_DIR/gcc-workspace"
BUILD_DIR="$WORKSPACE_DIR/gcc-build"
MIRROR="https://ftp.mpi-inf.mpg.de/mirrors/gnu/mirror/gcc.gnu.org/pub/gcc/releases"
VERSION=$1
GCC_SRC_DIR="$WORKSPACE_DIR/gcc-$VERSION"
CHECKSUM_FILE="sha512.sum"
INSTALL_DIR="$PWD/tools/gunwxcc_$VERSION"
TARGET=$2

set -e
set -x

if [[ "$OSTYPE" == "darwin"* ]]; then
    THREADS=$(sysctl -n hw.ncpu)
else
    THREADS=$(grep -c ^processor /proc/cpuinfo)
fi

# Install dependencies
echo "Step 1: Install dependencies"
if [[ "$OSTYPE" == "darwin"* ]]; then
    brew install wget
else 
    sudo apt update && sudo apt install build-essential m4 texinfo
fi

# Create directory structure
echo "Step 2: Creating directory structure"
rm -rf "$TEMP_DIR"
mkdir -p "$WORKSPACE_DIR"
cd "$WORKSPACE_DIR"

# Download sources
echo "Step 3: Downloading sources"
wget "$MIRROR/gcc-$VERSION/gcc-$VERSION.tar.gz"
wget "$MIRROR/gcc-$VERSION/$CHECKSUM_FILE"

# Verify checksum
echo "Step 4: Verifying checksum"
CHECKSUM="$(echo $(shasum -a512 gcc-$VERSION.tar.gz) | awk '{print $1}')"
EXPECTED_CHECKSUM="$(cat sha512.sum | grep gcc-$VERSION.tar.gz$ | awk '{print $1}')"

if [ $CHECKSUM != $EXPECTED_CHECKSUM ]; then
    echo "Invalid file checksum"
    exit 1
fi

# Unpack sources
echo "Step 5: Unpacking sources"
tar -xzf "gcc-$VERSION.tar.gz"

# # Download prerequisites
echo "Step 6: Downloading prerequisites"
cd "$GCC_SRC_DIR"
bash contrib/download_prerequisites
cd "$WORKSPACE_DIR"

# Download binutils
echo "Step 7: Downloading binutils"
BINUTILS_FILENAME="$(wget https://ftp.gnu.org/gnu/binutils/ -q -O - | grep -o -P "(?<=<a href=\").*(?=.tar.gz\">)" | sort --version-sort | tail -n 1).tar.gz"
BINUTILS_SRC_DIR="$WORKSPACE_DIR/$(awk -F.tar '{print $1}' <<< $BINUTILS_FILENAME)"
BINUTILS_URL="https://ftp.gnu.org/gnu/binutils/$BINUTILS_FILENAME"
BINUTILS_DIR="$GCC_SRC_DIR/binutils"
wget "$BINUTILS_URL"
tar -xzf "$BINUTILS_FILENAME"
mv "$BINUTILS_SRC_DIR" "$BINUTILS_DIR"

# Create build directory
echo "Step 8: Creating build directory"
cd "$WORKSPACE_DIR"
mkdir build-gcc
cd build-gcc

# Build
echo "Step 9: Build - configuring"
bash "../gcc-$VERSION/configure" "--prefix=$INSTALL_DIR" "--target=$TARGET" \
        --enable-languages=c,c++ --disable-multilib --disable-nls --without-headers \
        --with-gnu-ld

echo "Step 10: Build - building"
make "-j$THREADS"

# Install
echo "Step 11: Installing"
rm -rf $INSTALL_DIR
mkdir $INSTALL_DIR
make install

rm -rf "$TEMP_DIR"