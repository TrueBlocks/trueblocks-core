#!/usr/bin/env bash

#------------------------------------------------
# Name:    tag-all.sh
# Purpose: Tag all submodules with a semantic version
# Usage:   ./tag-all.sh <version>
# Example: ./tag-all.sh v6.4.6

#------------------------------------------------
# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

#------------------------------------------------
# Function to show help
show_help() {
    echo "USAGE: $0 <version>"
    echo "       $0 --help"
    echo
    echo "  <version>    Semantic version (vX.Y.Z)"
    echo
    echo "EXAMPLE: $0 v6.4.6"
}

#------------------------------------------------
# Validate arguments
if [ $# -eq 0 ] || [ "$1" = "--help" ] || [ "$1" = "-h" ]; then
    show_help
    exit 0
fi

if [ $# -ne 1 ]; then
    echo -e "${RED}Error: Invalid number of arguments${NC}"
    echo
    show_help
    exit 1
fi

VERSION="$1"

# Validate semantic version format (basic check)
if ! [[ "$VERSION" =~ ^v[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
    echo -e "${RED}Error: Invalid semantic version format${NC}"
    echo "Expected format: vX.Y.Z (e.g., v6.4.6)"
    exit 1
fi

echo -e "${YELLOW}[tag-all]${NC} Tagging all submodules with version: $VERSION"

#------------------------------------------------
# Get the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

#------------------------------------------------
# Find all git submodules
SUBMODULES=$(git submodule status | awk '{print $2}')

if [ -z "$SUBMODULES" ]; then
    echo -e "${RED}Error: No submodules found${NC}"
    exit 1
fi

echo -e "${YELLOW}[tag-all]${NC} Found submodules:"
for SUBMODULE in $SUBMODULES; do
    echo "  - $SUBMODULE"
done

echo
echo -e "${YELLOW}WARNING:${NC} This will FORCE TAG and FORCE PUSH version $VERSION to all submodules above."
echo -e "${YELLOW}WARNING:${NC} Any existing tags with this version will be overwritten."
echo
read -p "Are you sure you want to continue? (y/N): " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo -e "${YELLOW}[tag-all]${NC} Operation cancelled by user"
    exit 0
fi

echo

#------------------------------------------------
# Process each submodule
for SUBMODULE in $SUBMODULES; do
    echo -e "${YELLOW}[tag-all]${NC} Processing submodule: $SUBMODULE"
    
    # Check if submodule directory exists
    if [ ! -d "$SUBMODULE" ]; then
        echo -e "${RED}Error: Submodule directory $SUBMODULE does not exist${NC}"
        exit 1
    fi
    
    cd "$SUBMODULE" || exit 1
    
    # Check if we're in a git repository
    if ! git rev-parse --git-dir > /dev/null 2>&1; then
        echo -e "${RED}Error: $SUBMODULE is not a git repository${NC}"
        exit 1
    fi
    
    # Check if on main branch
    CURRENT_BRANCH=$(git branch --show-current)
    if [ "$CURRENT_BRANCH" != "main" ]; then
        echo -e "${RED}Error: $SUBMODULE is not on main branch (currently on: $CURRENT_BRANCH)${NC}"
        exit 1
    fi
    
    # Check for uncommitted changes
    if ! git diff-index --quiet HEAD --; then
        echo -e "${RED}Error: $SUBMODULE has uncommitted changes${NC}"
        git status --porcelain
        exit 1
    fi
    
    # Check for untracked files
    if [ -n "$(git status --porcelain)" ]; then
        echo -e "${RED}Error: $SUBMODULE has untracked or staged files${NC}"
        git status --porcelain
        exit 1
    fi
    
    # Force tag the repository
    echo -e "${YELLOW}[tag-all]${NC}   Tagging $SUBMODULE with $VERSION"
    if ! git tag -f "$VERSION"; then
        echo -e "${RED}Error: Failed to tag $SUBMODULE${NC}"
        exit 1
    fi
    
    # Force push the tag
    echo -e "${YELLOW}[tag-all]${NC}   Force pushing tag to remote"
    if ! git push -f origin "$VERSION"; then
        echo -e "${RED}Error: Failed to push tag for $SUBMODULE${NC}"
        exit 1
    fi
    
    echo -e "${GREEN}[tag-all]${NC}   Successfully tagged $SUBMODULE"
    
    # Return to root directory
    cd "$SCRIPT_DIR/.." || exit 1
done

echo
echo -e "${GREEN}[tag-all]${NC} Successfully tagged all submodules with version: $VERSION"
echo -e "${GREEN}[tag-all]${NC} Done."