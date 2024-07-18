#!/usr/bin/env bash

# This script lists all uses of either chifra or the sdk in the go.mod files. Used for debugging.
find . -type f -name 'go.mod' -exec grep -His "TrueBlocks" {} ';' | \
    grep "2024" | \
    cut -f2 -d: | \
    sed 's/ \/\/ indirect//' | \
    sed 's/require //' | \
    tr '\t' '|' | sed 's/|//' | sed 's/^/    /' | \
    sort -u
