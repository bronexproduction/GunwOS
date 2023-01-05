#!/bin/bash

if [ "$#" -ne 4 ]; then
    echo "Incorrect number of arguments"
fi

TARGET=$1
GCC_VERSION=$2
BINUTILS_VERSION=$3
NEWLIB_VERSION=$4

TEMP_DIR="$PWD/temp"
WORKSPACE_DIR="$TEMP_DIR/gcc-workspace"
BUILD_DIR="$WORKSPACE_DIR/gcc-build"
MIRROR="https://ftp.mpi-inf.mpg.de/mirrors/gnu/mirror/gcc.gnu.org/pub/gcc/releases"
GCC_SRC_DIR="$WORKSPACE_DIR/gcc-$GCC_VERSION"
CHECKSUM_FILE="sha512.sum"
TOOLS_DIR="$PWD/tools"

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
    brew install wget texinfo
else 
    sudo apt update && sudo apt install build-essential m4 texinfo bison
fi

# Create directory structure
echo "Step 2: Creating directory structure"
rm -rf "$TEMP_DIR"
mkdir -p "$WORKSPACE_DIR"
cd "$WORKSPACE_DIR"

# Download sources
echo "Step 3: Downloading sources"
wget "$MIRROR/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.gz"
wget "$MIRROR/gcc-$GCC_VERSION/$CHECKSUM_FILE"

# Verify checksum
echo "Step 4: Verifying checksum"
CHECKSUM="$(echo $(shasum -a512 gcc-$GCC_VERSION.tar.gz) | awk '{print $1}')"
EXPECTED_CHECKSUM="$(cat sha512.sum | grep gcc-$GCC_VERSION.tar.gz$ | awk '{print $1}')"

if [ $CHECKSUM != $EXPECTED_CHECKSUM ]; then
    echo "Invalid file checksum"
    exit 1
fi

# Unpack sources
echo "Step 5: Unpacking sources"
tar -xzf "gcc-$GCC_VERSION.tar.gz"

# Download prerequisites
echo "Step 6: Downloading prerequisites"
cd "$GCC_SRC_DIR"
bash contrib/download_prerequisites
cd "$WORKSPACE_DIR"

# Download binutils
echo "Step 7: Downloading binutils"
BINUTILS_FILENAME="binutils-$BINUTILS_VERSION.tar.gz"
BINUTILS_SRC_DIR="$WORKSPACE_DIR/binutils-$BINUTILS_VERSION"
BINUTILS_URL="https://ftp.gnu.org/gnu/binutils/$BINUTILS_FILENAME"
BINUTILS_DIR="$GCC_SRC_DIR/binutils"
wget "$BINUTILS_URL"
tar -xzf "$BINUTILS_FILENAME"

echo "Step 8: Downloading newlib"
NEWLIB_FILENAME="newlib-$NEWLIB_VERSION.tar.gz"
NEWLIB_SRC_DIR="$WORKSPACE_DIR/newlib-$NEWLIB_VERSION"
NEWLIB_URL="https://sourceware.org/ftp/newlib/$NEWLIB_FILENAME"
wget "$NEWLIB_URL"
tar -xzf "$NEWLIB_FILENAME"

# Create build directory
echo "Step 9: Creating build directory"
cd "$WORKSPACE_DIR"
BINUTILS_BUILD_DIR="$WORKSPACE_DIR/build-binutils"
GCC_BUILD_DIR="$WORKSPACE_DIR/build-gcc"
mkdir "$BINUTILS_BUILD_DIR"
mkdir "$GCC_BUILD_DIR"

INSTALL_DIR_NAME="gunwxcc-${GCC_VERSION}_binutils-${BINUTILS_VERSION}_newlib-${NEWLIB_VERSION}"
INSTALL_DIR="$TOOLS_DIR/$INSTALL_DIR_NAME"
TEMP_INSTALL_DIR="$WORKSPACE_DIR/$INSTALL_DIR_NAME"

# Build
echo "Step 10: Configuring binutils"
cd "$BINUTILS_BUILD_DIR"
bash "$BINUTILS_SRC_DIR/configure" "--prefix=$TEMP_INSTALL_DIR" "--target=$TARGET"

echo "Step 11: Building binutils"
make "-j$THREADS"

echo "Step 12: Installing binutils"
rm -rf $TEMP_INSTALL_DIR
mkdir -p $TEMP_INSTALL_DIR
make install

echo "Step 13: Configuring gcc"
cd "$GCC_BUILD_DIR"
bash "$GCC_SRC_DIR/configure" "--prefix=$TEMP_INSTALL_DIR" "--target=$TARGET" \
    --disable-multilib --disable-nls --without-headers \
    --enable-languages=c,c++ \
    --disable-libquadmath \
    --with-newlib "--with-headers=$NEWLIB_SRC_DIR/newlib/libc/include" \
    --with-gnu-as --with-gnu-ld

echo "Step 14: Building gcc"
make "-j$THREADS"

# Install
echo "Step 15: Installing gcc"
make install
rm -rf "$INSTALL_DIR"
mkdir -p "$TOOLS_DIR"
mv "$TEMP_INSTALL_DIR" "$INSTALL_DIR"

# Cleanup
echo "Step 16: Cleanup"
rm -rf "$TEMP_DIR"