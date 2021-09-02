#!/bin/bash

echo $(pwd)

# Build the binaries
mkdir build && cd build
cmake ../src
make -j 4

# Set the correct PATH
export PATH=$(pwd)/../bin:$(pwd)/../bin/test:$PATH

# Run test
make tests