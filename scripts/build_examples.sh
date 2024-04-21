#!/bin/bash

cd examples

echo "Building Example 1..."
go build -o ../bin/examples/example1 example1/main.go
example1

echo "Building Example 2..."
go build -o ../bin/examples/example2 example2/main.go
example2

echo "All examples built successfully."
