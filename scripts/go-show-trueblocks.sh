#!/usr/bin/env bash

# This script lists all uses of either chifra or the sdk in the go.mod files. Used for debugging.

# Navigate to the root of the repo
REPO_ROOT="$(git rev-parse --show-toplevel)"
cd "$REPO_ROOT"

find . -type f -name "go.mod" -exec grep -His TrueBlocks {} ';' | \
	grep -v module | \
	sed 's/ \/\/ indirect//' | \
	sed 's/require //' | \
	grep -v replace | \
	tr '\t' '|' | \
	sed 's/|//' | \
	cut -f2 -d: | \
	sort -u
