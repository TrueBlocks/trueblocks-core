#!/bin/bash

# pull-all.sh - Pull updates for all repositories
# This script visits each of the four repos and performs git pull --recurse-submodules

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to pull from a repository
pull_repo() {
    local repo_dir="$1"
    local repo_name="$2"
    
    print_status "Updating $repo_name..."
    
    if [ ! -d "$repo_dir" ]; then
        print_warning "Directory $repo_dir does not exist, skipping..."
        return 0
    fi
    
    if [ ! -d "$repo_dir/.git" ] && [ ! -f "$repo_dir/.git" ]; then
        print_warning "$repo_dir is not a git repository, skipping..."
        return 0
    fi
    
    cd "$repo_dir"
    
    # Check if we have any changes that might be lost
    if ! git diff --quiet || ! git diff --cached --quiet; then
        print_warning "$repo_name has uncommitted changes. Continuing anyway..."
    fi
    
    # Perform the pull
    if git pull --recurse-submodules; then
        print_status "$repo_name updated successfully"
    else
        print_error "Failed to update $repo_name"
        return 1
    fi
    
    cd - > /dev/null
}

#------------------------------------------------
# Get the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

#------------------------------------------------
cd $SCRIPT_DIR/..

print_status "Starting pull for all repositories..."
print_status "Repository root: $(pwd)"

# Array of repositories to update
repos=(
    "./build:build"
    "./sdk:sdk"  
    "./examples:examples"
    "./khedra:khedra"
)

# Track success/failure
failed_repos=()
success_count=0

# Pull from each repository
for repo_info in "${repos[@]}"; do
    IFS=':' read -r repo_dir repo_name <<< "$repo_info"
    
    if pull_repo "$repo_dir" "$repo_name"; then
        ((success_count++))
    else
        failed_repos+=("$repo_name")
    fi
    echo  # Add blank line for readability
done

# Print summary
echo "========================================="
print_status "Pull completed for $success_count repositories"

if [ ${#failed_repos[@]} -gt 0 ]; then
    print_error "Failed repositories: ${failed_repos[*]}"
    exit 1
else
    print_status "All repositories updated successfully!"
fi