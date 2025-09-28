#!/usr/bin/env bash

#------------------------------------------------
# Name:    go-work-sync.sh
# Purpose: Find go.mod files, save the directories
#          to a go.work file, and runs go work sync

#------------------------------------------------
# Get the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Navigate to the root of the repo
cd "$SCRIPT_DIR/.."

#------------------------------------------------
# Function to check if git submodules are initialized
check_submodules() {
    echo "===> Checking Git submodules..."
    if git submodule status | grep -q '^-'; then
        echo "Error: Some submodules are not initialized!"
        echo "Run: git submodule update --init --recursive"
        exit 1
    fi
}

# Run submodule check
check_submodules

#------------------------------------------------
# Check if the go.work file already exists
GO_WORK_FILE="go.work"
rm -f "$GO_WORK_FILE"
if [ ! -f "$GO_WORK_FILE" ]; then
    echo "// Go Version" > "$GO_WORK_FILE"
    echo "go 1.25.1" >> "$GO_WORK_FILE"
fi

#------------------------------------------------
# Find all go.mod files in the src directory and
# add their directories to go.work.
echo "===> Getting latest sdk and chifra..."
find . -type f -name 'go.mod' | while read -r modfile; do
    moddir=$(dirname "$modfile")
    go work use "$moddir"

    echo "    Updated $moddir/go.mod"
    cd "$moddir" || exit

    if [ "$moddir" = "./sdk" ]; then isSdk=true; else isSdk=false; fi
    if [ "$moddir" = "./src/apps/chifra" ]; then isChifra=true; else isChifra=false; fi
    if [ "$moddir" = "./src/dev_tools/goMaker" ]; then isGoMaker=true; else isGoMaker=false; fi
    if [ "$moddir" = "./node" ]; then isNode=true; else isNode=false; fi
    if [ "$moddir" = "./examples/four_bytes" ]; then isFourbyte=true; else isFourbyte=false; fi
    if [ "$moddir" = "./examples/keystore" ]; then isKeystore=true; else isKeystore=false; fi
    if [ "$moddir" = "./src/dev_tools/indexManager" ]; then isIndexMan=true; else isIndexMan=false; fi
    if [ "$moddir" = "./examples/simple" ]; then isSimple=true; else isSimple=false; fi

    if [ "$isGoMaker" = true ] || [ "$isNode" = true ] || [ "$isFourbyte" = true ]; then
        go get github.com/btcsuite/btcd 2> /dev/null
    fi

    if [ "$isSdk" = false ] && [ "$isChifra" = false ] && [ "$isGoMaker" = false ] && [ "$isFourbyte" = false ] && [ "$isIndexMan" = false ] && [ "$isKeystore" = false ]; then
        go get github.com/TrueBlocks/trueblocks-sdk/v5@latest
    fi

    if [ "$isChifra" = false ]; then
        go get github.com/TrueBlocks/trueblocks-core/src/apps/chifra@latest
    fi

    go mod tidy
    cd - > /dev/null
done

#------------------------------------------------
echo
echo "===> Creating a fresh go.work file..."
go work sync
cat go.work

#------------------------------------------------
echo
echo "===> Tidying go.mod files..."
"$SCRIPT_DIR/go-mod-tidy.sh"

echo
echo "===> The repo imports these TrueBlocks modules..."
"$SCRIPT_DIR/go-show-trueblocks.sh"

cd - 2>&1 > /dev/null
echo
echo "===> Done..."

exit 0
