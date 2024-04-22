#!/bin/bash

echo "Building examples"
pwd
cd examples
pwd
go build -o ../bin/examples/example1 example1/main.go
ls -l ../bin/examples/
