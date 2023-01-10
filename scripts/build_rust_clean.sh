#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Incorrect number of arguments"
fi

TARGET=$1
RUST_VERSION=$2

TEMP_DIR="$PWD/temp"
TOOLS_DIR="$PWD/tools"
WORKSPACE_DIR="$TEMP_DIR/rust-workspace"
XCOMP_BUILD_DIR="$WORKSPACE_DIR/rust-build"
LIBCORE_BUILD_DIR="$WORKSPACE_DIR/libcore-build"
RUST_GIT="https://github.com/rust-lang/rust.git"
RUST_LATEST_URL="https://github.com/rust-lang/rust/releases/latest"

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
    brew install git cmake ninja
else 
    sudo apt update && sudo apt install git curl build-essential cmake ninja-build
fi

# Create directory structure
echo "Step 2: Creating directory structure"
rm -rf "$TEMP_DIR"
mkdir -p "$WORKSPACE_DIR"
cd "$WORKSPACE_DIR"

# Download sources
echo "Step 3: Downloading sources"

if [[ "$RUST_VERSION" == "latest" ]]; then
    latest_version_url=$(curl -Ls -o /dev/null -w %{url_effective} "$RUST_LATEST_URL")
    RUST_VERSION=${latest_version_url##*/}
fi

XCOMP_INSTALL_DIR_NAME="rust-${RUST_VERSION}"
LIBCORE_COMPILER_INSTALL_DIR_NAME="libcore-compiler-${RUST_VERSION}"
LIBCORE_INSTALL_DIR_NAME="libcore-${RUST_VERSION}"
RUST_SRC_DIR="$WORKSPACE_DIR/$XCOMP_INSTALL_DIR_NAME"
XCOMP_INSTALL_DIR="$TOOLS_DIR/$XCOMP_INSTALL_DIR_NAME"
LIBCORE_COMPILER_INSTALL_DIR="$WORKSPACE_DIR/$LIBCORE_COMPILER_INSTALL_DIR_NAME"
TEMP_XCOMP_INSTALL_DIR="$WORKSPACE_DIR/temp-$XCOMP_INSTALL_DIR_NAME"
TEMP_LIBCORE_INSTALL_DIR="$WORKSPACE_DIR/temp-$LIBCORE_INSTALL_DIR_NAME"

git clone --depth 1 --branch "$RUST_VERSION" "$RUST_GIT" "$RUST_SRC_DIR"

# Create configuration
echo "Step 4: Creating Rust configuration"
mkdir "$XCOMP_BUILD_DIR" && cd "$XCOMP_BUILD_DIR"
"$RUST_SRC_DIR/configure" --prefix="$TEMP_XCOMP_INSTALL_DIR" --target="$TARGET" \
    --disable-docs

# Build
echo "Step 5: Building and installing"
rm -rf "$TEMP_XCOMP_INSTALL_DIR"
make "-j$THREADS" install

# Build core library
echo "Step 6: Building and installing core library"
mkdir "$LIBCORE_BUILD_DIR" && cd "$LIBCORE_BUILD_DIR"
"$RUST_SRC_DIR/configure" --prefix="$LIBCORE_COMPILER_INSTALL_DIR" --disable-docs
rm -rf "$LIBCORE_COMPILER_INSTALL_DIR"
make "-j$THREADS" install
"$LIBCORE_COMPILER_INSTALL_DIR/bin/rustc" "$RUST_SRC_DIR/library/core/src/lib.rs"

# Install
echo "Step 7: Installing"
rm -rf "$XCOMP_INSTALL_DIR"
mkdir -p "$TOOLS_DIR"
mv "$TEMP_XCOMP_INSTALL_DIR" "$XCOMP_INSTALL_DIR"

# Cleanup
echo "Step 8: Cleanup"
rm -rf "$TEMP_DIR"
