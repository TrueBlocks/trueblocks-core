#!/usr/bin/env bash

# yes | chifra rm  $1
# time chifra list $1 | wc
time acctScrape2 /Users/jrush/.quickBlocks/unchained/finalized $1 --max 4 --stream --log --fmt txt >output/$1.txt
