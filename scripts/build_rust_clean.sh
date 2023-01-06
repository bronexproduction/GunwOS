#!/bin/bash

TEMP_DIR="$PWD/temp"
WORKSPACE_DIR="$TEMP_DIR/rust-workspace"
BUILD_DIR="$WORKSPACE_DIR/rust-build"
MIRROR="https://github.com/rust-lang/rust.git"
RUST_SRC_DIR="$WORKSPACE_DIR/rust-$RUST_VERSION"
TOOLS_DIR="$PWD/tools"

set -e
set -x

# Install dependencies
echo "Step 1: Install dependencies"
if [[ "$OSTYPE" == "darwin"* ]]; then
    brew install git
else 
    sudo apt update && sudo apt install git
fi

# Create directory structure
echo "Step 2: Creating directory structure"
rm -rf "$TEMP_DIR"
mkdir -p "$WORKSPACE_DIR"
cd "$WORKSPACE_DIR"

# Download sources
echo "Step 3: Downloading sources"
git clone "$MIRROR"
















# Cleanup
echo "Step 16: Cleanup"
rm -rf "$TEMP_DIR"
