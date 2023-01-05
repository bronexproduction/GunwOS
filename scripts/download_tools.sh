#!/bin/bash

set -e
set -x

if [[ "$OSTYPE" == "darwin"* ]]; then
    brew install python
    pip3 install gdown
else
    sudo apt install python3-pip python3-testresources
    pip install gdown
fi

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

gdown "$TOOLS_ID" -O tools.zip
rm -rf tools
unzip tools.zip
rm tools.zip
