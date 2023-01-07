#!/bin/bash

RUST_VERSION=$1

TEMP_DIR="$PWD/temp"
WORKSPACE_DIR="$TEMP_DIR/rust-workspace"
BUILD_DIR="$WORKSPACE_DIR/rust-build"
RUST_FILENAME="$RUST_VERSION.tar.gz"
RUST_URL="https://github.com/rust-lang/rust/archive/refs/tags/$RUST_FILENAME"
RUST_SRC_DIR="$WORKSPACE_DIR/rust-$RUST_VERSION"
TOOLS_DIR="$PWD/tools"

set -e
set -x

# Install dependencies
echo "Step 1: Install dependencies"
if [[ "$OSTYPE" == "darwin"* ]]; then
    brew install wget
else 
    # sudo apt update && sudo apt install
fi

# Create directory structure
echo "Step 2: Creating directory structure"
rm -rf "$TEMP_DIR"
mkdir -p "$WORKSPACE_DIR"
cd "$WORKSPACE_DIR"

# Download sources
echo "Step 3: Downloading sources"
wget "$RUST_URL"

# Unpack sources
echo "Step 4: Unpacking sources"
tar -xzf "$RUST_FILENAME"

# Create build directory
echo "Step 5: Creating build directory"
mkdir "$BUILD_DIR"

# Create configuration
echo "Step 6: Creating Rust configuration"
cd "$BUILD_DIR"
bash "$RUST_SRC_DIR/x" setup compiler

# Build
echo "Step 7: Building Rust compiler"
bash "$RUST_SRC_DIR/x" build compiler


exit 4






# Cleanup
echo "Step 16: Cleanup"
rm -rf "$TEMP_DIR"
