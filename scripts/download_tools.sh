#!/bin/bash

set -e

QUERY=`curl -s -L "https://drive.google.com/uc?export=download&id=1lFFKgdm-P1xWkPZV1mojCOeyCxg9KSrK" \
    | pup 'form#downloadForm attr{action}' \
    | sed -e 's/amp;//g'` 
    
curl -L -o tools.zip "${QUERY}"
unzip tools.zip
rm tools.zip
