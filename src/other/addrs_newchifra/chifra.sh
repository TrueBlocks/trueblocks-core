#!/bin/bash

[ -z "$QB_MONITORS" ] && echo "Need to set QB_MONITORS" && exit 1;

address=`echo $1 | tr [:upper:] [:lower:]`
[ -z "$address" ] && echo "You must provide an address: ./maker <name> <address>" && exit 1;

name=$2
[ -z "$name" ] && echo "You must provide a name: ./maker <name> <address>" && exit 1;

path=monitors/$name
base_path=`pwd`
result=$QB_MONITORS/test/gold/monitors/txt/$name-$address.txt
extractor=$base_path/../addrs_extraction_3/bin/addrs_query
#echo $name
#echo $address
#echo $path
#echo $base_path
#echo $result
#echo $extractor
#exit

lastBlock=0
if [ -e $path/config.toml ]
then
    colorEcho -c green -t "Refreshing monitor for address" | tr '\n' ' '
    colorEcho -c teal -t $address
    cd $path
    lastBlock=`cat cache/lastBlock.txt`
    echo $lastBlock
else
    colorEcho -c green -t "Building monitor for address" | tr '\n' ' '
    colorEcho -c teal -t $address
    lastBlock=`isContract -w $address 2>/dev/null | cut -f2 -d' '`
    if [[ -z "${lastBlock// }" ]]
    then
        lastBlock=0
    fi
    echo $lastBlock | sed 's/^/s\/FIRST\//' | sed 's/$/\//' >z
    mkdir -p $path
    cd $path
    mkdir -p cache
    cp -pR $base_path/bin .
    cat $base_path/config.toml | sed 's/ADDR/'$address'/' | sed 's/NAME/'$name'/' | sed -f $base_path/z >config.toml
    rm $base_path/z
    # just in case
    cat config.toml | sed 's/FIRST/0/' >z ; cat z >config.toml
fi

colorEcho -c green -t "Scanning chain for appearances of $address..."
echo $extractor $address $lastBlock
$extractor $address $lastBlock >file
colorEcho -c green -t "Creating import..."
cat file | tr '.' '\t' >import.txt
cacheMan -i | grep -v "^$"
rm -f z file import.bak

colorEcho -c green -t "Exporting..."
acctExport --fmt txt | head -100 2>/dev/null
colorEcho -c green -t "...and many more"
echo
#acctExport 2>/dev/null | tee $result
#colorEcho -c green -t "JSON file written to $result"
