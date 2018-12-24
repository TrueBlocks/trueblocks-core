#!/usr/bin/env bash

echo "starting getBlock $1"

#getBlock --addrs  "$1" >"$1-unsorted_allad".txt &
#getBlock --uniq   "$1" >"$1-unsorted_block".txt &
getBlock --uniqTx "$1" >"$1-unsorted_trans".txt &
