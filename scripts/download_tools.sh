#!/bin/bash

set -e

if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    if [[ "$(uname -m)" == "x86_64" ]]; then
        TOOLS_ID=
    else
        TOOLS_ID=
    fi    
else
    # Linux
    TOOLS_ID=1lFFKgdm-P1xWkPZV1mojCOeyCxg9KSrK
fi

QUERY=`curl -s -L "https://drive.google.com/uc?export=download&id=$TOOLS_ID" \
    | pup 'form#downloadForm attr{action}' \
    | sed -e 's/amp;//g'` 
    
curl -L -o tools.zip "${QUERY}"
unzip tools.zip
rm tools.zip
