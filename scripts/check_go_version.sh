#!/usr/bin/env bash

# Name:    check_go_version.sh
# Purpose: Check that the Go version in Dockerfile matches the required version of Go

#------------------------------------------------
# Check if required version parameter is provided
if [ $# -ne 1 ]; then
    echo "❌ ERROR: Usage: $0 <required_go_version>"
    echo "   Example: $0 1.25.1"
    exit 1
fi

REQUIRED_GO_VERSION="$1"

#------------------------------------------------
# Get the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DOCKERFILE_PATH="$SCRIPT_DIR/Dockerfile"

#------------------------------------------------
# Check if Dockerfile exists
if [ ! -f "$DOCKERFILE_PATH" ]; then
    echo "❌ ERROR: Dockerfile not found at $DOCKERFILE_PATH"
    exit 1
fi

#------------------------------------------------
# Extract Go version from Dockerfile FROM line
DOCKER_GO_VERSION=$(grep "FROM golang:" "$DOCKERFILE_PATH" | grep -o '[0-9][0-9.]*' | head -1)

if [ -z "$DOCKER_GO_VERSION" ]; then
    echo "❌ ERROR: Could not extract Go version from Dockerfile"
    echo "   Looking for line starting with 'FROM golang:' in $DOCKERFILE_PATH"
    exit 1
fi

#------------------------------------------------
# Compare versions
echo "🔍 Checking Go version compatibility..."
echo "   Required version: $REQUIRED_GO_VERSION"
echo "   Dockerfile version: $DOCKER_GO_VERSION"

if [ "$DOCKER_GO_VERSION" != "$REQUIRED_GO_VERSION" ]; then
    echo "❌ CRITICAL ERROR: Go version mismatch!"
    echo "   Workflow expects: $REQUIRED_GO_VERSION"
    echo "   Dockerfile has: $DOCKER_GO_VERSION"
    echo "   Please update $DOCKERFILE_PATH FROM line to use golang:$REQUIRED_GO_VERSION-bullseye"
    exit 1
fi

echo "✅ Go version check passed"
exit 0
