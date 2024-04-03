#!/usr/bin/env bash

chifra tokens 0xbb9bc244d798123fde783fcc1c72d3bb8c189413 0x2910543af39aba0cd09dbb2d50200b3e800a63d2 --fmt json | jq >tmp.tokens
cat tmp.tokens | sed 's/^/                 /' >tokens.txt
rm -f tmp.tokens

chifra abis 0x5c69bee701ef814a2b6a3edd4b1652cb9cc5aa6f --fmt json | jq '.[] | .[] | select(.name=="PairCreated")' | ./wrap.sh | jq >tmp.abis
cat tmp.abis | sed 's/^/                 /' >abis.txt
rm -f tmp.abis

cd ../../../../build/ ; make generate finish ; cd -

