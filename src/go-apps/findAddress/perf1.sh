#!/usr/bin/env bash

# yes | chifra rm  $1
# time chifra list $1 | wc
time ./findAddress /Users/jrush/.quickBlocks/cache/addr_index/finalized $1 --max 4 --stream --log --fmt txt >output/$1.txt
