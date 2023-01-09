#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Incorrect number of arguments"
fi

TARGET=$1
RUST_VERSION=$2

TEMP_DIR="$PWD/temp"
TOOLS_DIR="$PWD/tools"
WORKSPACE_DIR="$TEMP_DIR/rust-workspace"
BUILD_DIR="$WORKSPACE_DIR/rust-build"
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

INSTALL_DIR_NAME="rust-${RUST_VERSION}"
RUST_SRC_DIR="$WORKSPACE_DIR/$INSTALL_DIR_NAME"
INSTALL_DIR="$TOOLS_DIR/$INSTALL_DIR_NAME"
TEMP_INSTALL_DIR="$WORKSPACE_DIR/temp-$INSTALL_DIR_NAME"

git clone --depth 1 --branch "$RUST_VERSION" "$RUST_GIT" "$RUST_SRC_DIR"

# Create build directory
echo "Step 4: Creating build directory"
mkdir "$BUILD_DIR"

# Create configuration
echo "Step 5: Creating Rust configuration"
cd "$BUILD_DIR"
"$RUST_SRC_DIR/configure" --prefix="$TEMP_INSTALL_DIR" --target="$TARGET" \
    --disable-docs

# Install
echo "Step 6: Building and installing"
rm -rf "$TEMP_INSTALL_DIR"
make "-j$THREADS" install
rm -rf "$INSTALL_DIR"
mkdir -p "$TOOLS_DIR"
mv "$TEMP_INSTALL_DIR" "$INSTALL_DIR"

# Cleanup
echo "Step 7: Cleanup"
rm -rf "$TEMP_DIR"
