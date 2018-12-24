#!/usr/bin/env bash

[ -z "$QB_MONITORS" ] && echo "Need to set QB_MONITORS" && exit 1;

address=$1
name=`basename ${PWD}`
filename=$name-$address.txt
GOLD=${QB_MONITORS}/test/gold/chifra/txt

echo "addr: " $address
echo "name: " $name
echo "filename: " $filename
echo "gold: " $GOLD
echo "path: " $GOLD/$filename
exit
