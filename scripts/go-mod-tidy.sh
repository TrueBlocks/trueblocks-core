#!/usr/bin/env bash

#------------------------------------------------
# Name:    go-mod-tidy.sh
# Purpose: Finds all go.mod and tidys them

#------------------------------------------------
# Colors for output
CYAN='\033[0;36m'
NC='\033[0m' # No Color

#------------------------------------------------
# Get the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

#------------------------------------------------
cd $SCRIPT_DIR/..

#------------------------------------------------
find . -type f -name 'go.mod' | while read -r modfile; do
    moddir=$(dirname "$modfile")
    echo -e "  ${CYAN}[go-mod-tidy]${NC} $moddir"
    cd "$moddir" ; go mod tidy ; cd - >/dev/null
done
