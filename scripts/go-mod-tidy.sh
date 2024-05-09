#!/usr/bin/env bash

#------------------------------------------------
# Name:    go-mod-tidy.sh
# Purpose: Finds all go.mod and tidys them

#------------------------------------------------
# Get the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

#------------------------------------------------
cd $SCRIPT_DIR/../sdk                      ; go mod tidy
cd $SCRIPT_DIR/../examples/simple          ; go mod tidy
cd $SCRIPT_DIR/../examples/balanceChart    ; go mod tidy
cd $SCRIPT_DIR/../examples/findFirst       ; go mod tidy
cd $SCRIPT_DIR/../src/dev_tools/testRunner ; go mod tidy
cd $SCRIPT_DIR/../src/dev_tools/goMaker    ; go mod tidy
cd $SCRIPT_DIR/../src/dev_tools/sdkFuzzer  ; go mod tidy
cd $SCRIPT_DIR/../src/apps/chifra          ; go mod tidy
