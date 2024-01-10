#!/bin/bash

set -e
set -x

ARCH=$(uname -m)

if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    if [[ "$ARCH" == "x86_64" ]]; then
        TOOLS_ID=1w17A9ZFjoRMMtxyVQItj0KvqCjhsdtTF
    else
        TOOLS_ID=1bo9BwLs_HF9UcCaXWXEDess6__xXpBP4
    fi    
else
    # Linux
    if [[ "$ARCH" == "x86_64" ]]; then
    	TOOLS_ID=1-e8cyz_aMVlYcX5I7LyHgV2JNmd7Wu65
    elif [[ "$ARCH" == "i686" ]]; then
        TOOLS_ID=1ymPFp3nLYnFQ0Zp9lFYrNhS2oVWCp_EQ
    elif [[ "$ARCH" == "aarch64" ]]; then
        TOOLS_ID=1-Kd40OUYHd414LsORV7IJjViotGH6Lds
    else 
        TOOLS_ID=
    fi
fi

if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    brew install python
    pip3 install gdown
    GDOWN="gdown"
else
    # Linux
    sudo apt install python3-pip python3-testresources
    pip3 install gdown
    # Workaround for Visual Studio Code
    GDOWN="$HOME/.local/bin/gdown"
fi

BINDGEN_VERSION=0.69.1
export CARGO_HOME="tools/rust-bindgen-$BINDGEN_VERSION"
curl --proto '=https' --tlsv1.2 -LsSf https://github.com/rust-lang/rust-bindgen/releases/download/v$BINDGEN_VERSION/bindgen-cli-installer.sh | sh

rm -rf temp && mkdir temp && cd temp
"$GDOWN" "$TOOLS_ID" -O tools.zip
unzip tools.zip
rm -rf ../tools && mv tools ../tools
cd ..
rm -rf temp
