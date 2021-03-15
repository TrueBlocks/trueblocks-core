#!/usr/bin/env bash

chifra export --logs --articulate --cache_txs --cache_traces --fmt json $1 >data/$1.json
chifra export --logs --articulate --cache_txs --cache_traces --fmt csv $1 >data/$1.csv
sleep .2
