#!/usr/bin/env bash

#------------------------------------------------
# Name:    go-mod-tidy.sh
# Purpose: Finds all go.mod and tidys them

#------------------------------------------------
# Get the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

#------------------------------------------------
cd $SCRIPT_DIR/..

#------------------------------------------------
find . -type f -name 'go.mod' | while read -r modfile; do
    moddir=$(dirname "$modfile")
    echo "    Tidying $moddir"
    cd "$moddir" ; go mod tidy ; cd - >/dev/null
done
