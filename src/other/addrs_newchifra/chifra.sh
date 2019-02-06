#!/bin/bash

[ -z "$QB_MONITORS" ] && echo "Need to set QB_MONITORS" && exit 1;

address=`echo $1 | tr [:upper:] [:lower:]`
[ -z "$address" ] && echo "You must provide an address: ./chifra.sh <address> <name>" && exit 1;

name=$2
[ -z "$name" ] && echo "You must provide a name: ./chifra.sh <address> <name>" && exit 1;

path=monitors/$name
base_path=`pwd`
#echo "name: " $name
#echo "address: " $address
#echo "path: " $path
#echo "base_path: " $base_path
#exit

colorEcho -c green -t "Building monitor for address" | tr '\n' ' '
colorEcho -c teal -t $address
mkdir -p $path 2>/dev/null
mkdir -p $path/cache 2>/dev/null
cd $path
#cp -pR $base_path/bin .
cat $base_path/config.toml | sed 's/ADDR/'$address'/' | sed 's/NAME/'$name'/' >config.toml

colorEcho -c green -t "Scanning chain for appearances of $address..."
echo $address
acctScrape --list >import.txt
cacheMan -i | grep -v "^$"
rm -f import.bak

colorEcho -c green -t "Exporting with 'acctExport --fmt txt'"
acctExport --fmt txt | head -100 2>/dev/null
colorEcho -c green -t "...and many more"
echo
