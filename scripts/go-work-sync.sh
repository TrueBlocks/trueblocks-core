#!/usr/bin/env bash

#------------------------------------------------
# Name:    go-work-sync.sh
# Purpose: Find go.mod files, save the directories
#          to a go.work file, and runs go work sync

#------------------------------------------------
# Colors for output
BLUE='\033[0;34m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

#------------------------------------------------
# Get the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Navigate to the root of the repo
cd "$SCRIPT_DIR/.."

#------------------------------------------------
# Function to check if git submodules are initialized
check_submodules() {
    echo -e "${BLUE}[go-work-sync]${NC} Checking submodules..."
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
# Find all go.mod files and add their directories to go.work
echo -e "${BLUE}[go-work-sync]${NC} Updating module dependencies..."
find . -type f -name 'go.mod' | while read -r modfile; do
    moddir=$(dirname "$modfile")
    go work use "$moddir"

    echo "    Updated $moddir/go.mod"
    cd "$moddir" || exit

    if [ "$moddir" = "./sdk" ]; then isSdk=true; else isSdk=false; fi
    if [ "$moddir" = "./chifra" ]; then isChifra=true; else isChifra=false; fi
    if [ "$moddir" = "./dev-tools/goMaker" ]; then isGoMaker=true; else isGoMaker=false; fi
    if [ "$moddir" = "./dev-tools/testRunner" ]; then isTestRunner=true; else isTestRunner=false; fi
    if [ "$moddir" = "./khedra" ]; then isKhedra=true; else isKhedra=false; fi
    if [ "$moddir" = "./node" ]; then isNode=true; else isNode=false; fi
    
    # Detect blockchain-heavy examples that need btcsuite/btcd
    if [[ "$moddir" =~ ^./examples/ ]] && grep -q "github.com/ethereum/go-ethereum" "$moddir/go.mod" 2>/dev/null; then
        isBlockchainExample=true
    else
        isBlockchainExample=false
    fi
    
    # Detect SDK-based examples  
    if [[ "$moddir" =~ ^./examples/ ]] && grep -q "github.com/TrueBlocks/trueblocks-sdk/v6" "$moddir/go.mod" 2>/dev/null; then
        isSdkExample=true
    else
        isSdkExample=false
    fi

    if [ "$isTestRunner" = true ] || [ "$isKhedra" = true ] || [ "$isNode" = true ] || [ "$isBlockchainExample" = true ]; then
        go get github.com/btcsuite/btcd >/dev/null 2>&1
    fi

    if [ "$isSdk" = false ] && [ "$isChifra" = false ] && [ "$isGoMaker" = false ] && [ "$isTestRunner" = false ] && [ "$isKhedra" = false ] && [ "$isBlockchainExample" = false ] && [ "$isIndexMan" = false ] && [ "$isSdkExample" = false ]; then
        go get github.com/TrueBlocks/trueblocks-sdk/v6@latest
    fi

    if [ "$isChifra" = false ]; then
        go get github.com/TrueBlocks/trueblocks-chifra/v6@latest
    fi

    go mod tidy >/dev/null 2>&1
    cd - > /dev/null
done

#------------------------------------------------
echo
echo -e "${BLUE}[go-work-sync]${NC} Creating go.work file..."
go work sync
cat go.work

#------------------------------------------------
echo
echo -e "${BLUE}[go-work-sync]${NC} Running go-mod-tidy..."
"$SCRIPT_DIR/go-mod-tidy.sh"

echo
echo -e "${BLUE}[go-work-sync]${NC} Showing TrueBlocks modules..."
"$SCRIPT_DIR/go-show-trueblocks.sh"

cd - 2>&1 > /dev/null
echo
echo -e "${GREEN}[go-work-sync]${NC} Done."

exit 0
