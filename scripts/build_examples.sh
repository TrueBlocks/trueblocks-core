#!/bin/bash

cd examples
go build -o ../bin/examples/simple simple/main.go
go build -o ../bin/examples/balanceChart balanceChart/main.go
