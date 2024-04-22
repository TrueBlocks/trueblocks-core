#!/bin/bash

# Function to build a Go program
build_target() {
    echo "Build target $1"
    go build -o "../bin/examples/$1" "$1/main.go"
}

# Change to the examples directory
cd examples

# Build each target using the build_target function
build_target simple
build_target findFirst
build_target balanceChart
