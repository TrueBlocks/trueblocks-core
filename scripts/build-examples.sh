#!/bin/bash

#------------------------------------------------
# Name:    build-examples.sh
# Purpose: This script sets up the build environment,
#          builds the project, and runs the tests.

#------------------------------------------------
# Function to build a Go program
build_target() {
    echo "Building target $1"
    (cd "$1" && go build -o "../../bin/examples/$1")
}

#------------------------------------------------
# Build each target using the build_target function
cd examples
build_target simple
build_target findFirst
build_target balanceChart
