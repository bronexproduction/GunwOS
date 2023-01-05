#!/bin/bash

set -e

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

QUERY=`curl -s -L "https://drive.google.com/uc?export=download&id=$TOOLS_ID" \
    | pup 'form#downloadForm attr{action}' \
    | sed -e 's/amp;//g'` 
    
curl -L -o tools.zip "${QUERY}"
unzip tools.zip
rm tools.zip
