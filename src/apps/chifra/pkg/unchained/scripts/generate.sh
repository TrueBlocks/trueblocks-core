#!/bin/bash

SOURCE=${UNCHAINED_SOURCE-"UnchainedIndex.multichain.sol"}
OUTDIR=${UNCHAINED_OUT-"./build"}
GOLANGDIR=${UNCHAINED_GOLANGDIR-"./unchained"}

solc --optimize --abi $SOURCE -o $OUTDIR
solc --optimize --bin $SOURCE -o $OUTDIR
abigen --abi=$OUTDIR/UnchainedIndex.abi --bin=$OUTDIR/UnchainedIndex.bin --pkg=unchained --out=$GOLANGDIR/unchainedindex.multichain.go