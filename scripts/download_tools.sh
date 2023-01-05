#!/bin/bash

set -e
set -x

TARGET="$HOME/.local/"

sudo apt install python3-pip python3-testresources
pip install --upgrade gdown

if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    if [[ "$(uname -m)" == "x86_64" ]]; then
        TOOLS_ID=1-81LfQguWHVuIF5ec_2qPr_gnGpRGwDq
    else
        TOOLS_ID=1-81LfQguWHVuIF5ec_2qPr_gnGpRGwDq
    fi    
else
    # Linux
    TOOLS_ID=1-e8cyz_aMVlYcX5I7LyHgV2JNmd7Wu65
fi

"$TARGET/bin/gdown" "$TOOLS_ID" -O tools.zip
rm -rf tools
unzip tools.zip
rm tools.zip
