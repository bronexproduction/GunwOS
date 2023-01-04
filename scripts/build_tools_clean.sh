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

THREADS=$(grep -c ^processor /proc/cpuinfo)

# Install dependencies
echo "Step 1: Install dependencies"
sudo apt update && sudo apt install build-essential m4

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

# Download dependencies
echo "Step 6: Downloading dependencies"

# libgmp
LIBGMP_URL=$(wget https://gmplib.org/ -q -O - | grep -o "<a href=\"https://.*tar.xz</a>" | head -n 1 | awk -F\" '{printf $2}')
LIBGMP_FILENAME=$(grep -o gmp-.*tar.xz <<< "$LIBGMP_URL")
LIBGMP_SRC_DIR="$WORKSPACE_DIR/$(awk -F.tar '{print $1}' <<< $LIBGMP_FILENAME)"
LIBGMP_DIR="$GCC_SRC_DIR/gmp"
wget "$LIBGMP_URL"
tar -xf "$LIBGMP_FILENAME"
mv "$LIBGMP_SRC_DIR" "$LIBGMP_DIR"

# libmpfr
LIBMPFR_VERSION=$(wget https://www.mpfr.org/mpfr-current -q -O - | grep -o -P "(?<=<title>GNU MPFR version ).*(?=</title>)")
LIBMPFR_SRC_DIR="$WORKSPACE_DIR/mpfr-$LIBMPFR_VERSION"
LIBMPFR_FILENAME="mpfr-$LIBMPFR_VERSION.tar.gz"
LIBMPFR_URL="https://www.mpfr.org/mpfr-current/$LIBMPFR_FILENAME"
LIBMPFR_DIR="$GCC_SRC_DIR/mpfr"
wget "$LIBMPFR_URL"
tar -xzf "$LIBMPFR_FILENAME"
mv "$LIBMPFR_SRC_DIR" "$LIBMPFR_DIR"

# libmpc
LIBMPC_FILENAME=$(wget https://ftp.gnu.org/gnu/mpc/ -q -O - | grep -o "<a href=\"mpc-.*.tar.gz\"" | awk -F\" '{print $2}' | sort --version-sort | tail -n 1)
LIBMPC_URL="https://ftp.gnu.org/gnu/mpc/$LIBMPC_FILENAME"
LIBMPC_SRC_DIR="$WORKSPACE_DIR/$(awk -F.tar '{print $1}' <<< $LIBMPC_FILENAME)"
LIBMPC_DIR="$GCC_SRC_DIR/mpc"
wget "$LIBMPC_URL"
tar -xzf "$LIBMPC_FILENAME"
mv "$LIBMPC_SRC_DIR" "$LIBMPC_DIR"

# Create build directory
echo "Step 7: Creating build directory"
cd "$WORKSPACE_DIR"
mkdir build-gcc
cd build-gcc

# Build
echo "Step 8: Build - configuring"
bash "../gcc-$VERSION/configure" "--prefix=$INSTALL_DIR" "--target=$TARGET" \
        --enable-languages=c,c++ --disable-multilib --disable-nls --without-headers \
        --with-gnu-ld

echo "Step 9: Build - building"
make "-j$THREADS"

# Install
echo "Step 10: Installing"
rm -rf $INSTALL_DIR
mkdir $INSTALL_DIR
make install