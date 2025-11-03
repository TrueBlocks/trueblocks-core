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
    echo "       $0 show"
    echo "       $0 --help"
    echo
    echo "  <version>    Semantic version (vX.Y.Z)"
    echo "  show         Show latest tags for all repos"
    echo
    echo "EXAMPLE: $0 v6.4.6"
    echo "         $0 show"
}

#------------------------------------------------
# Validate arguments
if [ $# -eq 0 ] || [ "$1" = "--help" ] || [ "$1" = "-h" ]; then
    show_help
    exit 0
fi

if [ "$1" = "show" ]; then
    # Show mode - display latest tags
    echo -e "${YELLOW}[tag-all]${NC} Showing latest tags:"
    echo
    
    # Show root repo tag
    echo -e "${GREEN}Root repository:${NC}"
    LATEST_TAG=$(git describe --tags --abbrev=0 2>/dev/null || echo "No tags found")
    echo "  Latest tag: $LATEST_TAG"
    echo
    
    # Show submodule tags
    SUBMODULES=$(git submodule status | awk '{print $2}')
    if [ -n "$SUBMODULES" ]; then
        echo -e "${GREEN}Submodules:${NC}"
        for SUBMODULE in $SUBMODULES; do
            if [ -d "$SUBMODULE" ]; then
                cd "$SUBMODULE" || continue
                LATEST_TAG=$(git describe --tags --abbrev=0 2>/dev/null || echo "No tags found")
                echo "  $SUBMODULE: $LATEST_TAG"
                cd "$SCRIPT_DIR/.." || exit 1
            fi
        done
    fi
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
echo -e "${YELLOW}WARNING:${NC} This will FORCE TAG and FORCE PUSH version $VERSION to:"
echo "  - Root repository (trueblocks-core)"
echo "  - All submodules listed above"
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
# Tag and push root repository first
echo -e "${YELLOW}[tag-all]${NC} Processing root repository"

# Check if root is on main branch
CURRENT_BRANCH=$(git branch --show-current)
if [ "$CURRENT_BRANCH" != "main" ]; then
    echo -e "${RED}Error: Root repository is not on main branch (currently on: $CURRENT_BRANCH)${NC}"
    exit 1
fi

# Check for uncommitted changes in root
if ! git diff-index --quiet HEAD --; then
    echo -e "${RED}Error: Root repository has uncommitted changes${NC}"
    git status --porcelain
    exit 1
fi

# Check for untracked files in root
if [ -n "$(git status --porcelain)" ]; then
    echo -e "${RED}Error: Root repository has untracked or staged files${NC}"
    git status --porcelain
    exit 1
fi

# Force tag the root repository
echo -e "${YELLOW}[tag-all]${NC}   Tagging root repository with $VERSION"
if ! git tag -f "$VERSION"; then
    echo -e "${RED}Error: Failed to tag root repository${NC}"
    exit 1
fi

# Force push the tag from root
echo -e "${YELLOW}[tag-all]${NC}   Force pushing root tag to remote"
if ! git push -f origin "$VERSION"; then
    echo -e "${RED}Error: Failed to push tag for root repository${NC}"
    exit 1
fi

echo -e "${GREEN}[tag-all]${NC}   Successfully tagged root repository"

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
echo -e "${GREEN}[tag-all]${NC} Successfully tagged root repository and all submodules with version: $VERSION"
echo -e "${GREEN}[tag-all]${NC} Done."