#!/bin/bash

RUST_VERSION=$1

TEMP_DIR="$PWD/temp"
WORKSPACE_DIR="$TEMP_DIR/rust-workspace"
BUILD_DIR="$WORKSPACE_DIR/rust-build"
RUST_GIT="https://github.com/rust-lang/rust.git"
RUST_FILENAME="$RUST_VERSION.tar.gz"
RUST_URL="https://github.com/rust-lang/rust/archive/refs/tags/$RUST_FILENAME"
RUST_SRC_DIR="$WORKSPACE_DIR/rust-$RUST_VERSION"
TOOLS_DIR="$PWD/tools"

set -e
set -x

# Install dependencies
echo "Step 1: Install dependencies"
if [[ "$RUST_VERSION" == "latest" ]]; then 
    DEPS=("git")
    DOWNLOAD="git"
    DOWNLOAD_PARAMS=("clone" "$RUST_GIT")
    UNPACK="mv"
    UNPACK_PARAMS=("rust" "rust-latest")
else
    DEPS=("wget")
    DOWNLOAD="wget"
    DOWNLOAD_PARAMS=("$RUST_URL")
    UNPACK="tar"
    UNPACK_PARAMS=("-xzf" "$RUST_FILENAME")
fi
    
if [[ "$OSTYPE" == "darwin"* ]]; then
    brew install ${DEPS[@]}
else 
    sudo apt update && sudo apt ${DEPS[@]}
fi

# Create directory structure
echo "Step 2: Creating directory structure"
rm -rf "$TEMP_DIR"
mkdir -p "$WORKSPACE_DIR"
cd "$WORKSPACE_DIR"

# Download sources
echo "Step 3: Downloading sources"
"$DOWNLOAD" ${DOWNLOAD_PARAMS[@]}

# Unpack sources
echo "Step 4: Unpacking sources"
"$UNPACK" ${UNPACK_PARAMS[@]}

# Create build directory
echo "Step 5: Creating build directory"
mkdir "$BUILD_DIR"

INSTALL_DIR_NAME="rust-${RUST_VERSION}"
INSTALL_DIR="$TOOLS_DIR/$INSTALL_DIR_NAME"
TEMP_INSTALL_DIR="$WORKSPACE_DIR/$INSTALL_DIR_NAME"

# Create configuration
echo "Step 6: Creating Rust configuration"
cd "$BUILD_DIR"
bash "$RUST_SRC_DIR/x" setup compiler

# Build
echo "Step 7: Building Rust compiler"
bash "$RUST_SRC_DIR/x" build compiler

# Install
echo "Step 8: Installing Rust"
# TODO: configure prefix
# rm -rf $TEMP_INSTALL_DIR
# mkdir -p $TEMP_INSTALL_DIR
# bash "$RUST_SRC_DIR/x" install compiler
# rm -rf "$INSTALL_DIR"
# mkdir -p "$TOOLS_DIR"
# mv "$TEMP_INSTALL_DIR" "$INSTALL_DIR"


exit 4






# Cleanup
echo "Step 16: Cleanup"
rm -rf "$TEMP_DIR"
