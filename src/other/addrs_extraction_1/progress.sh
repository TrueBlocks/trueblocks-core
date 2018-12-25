#!/usr/bin/env bash

clear
ps -ef | grep getBlock | grep -v grep | grep -v latest

tail -n 1 *.txt | grep -v "^$" | sed 's/==>/|/' | tr '\n' ' ' | tr '|' '\n' | tr '\t' ' '  | sed 's/<==/==>/'
#echo
#echo
#tail -n 2 *.txt
echo
du -hm * 2>&1 | sort -n | grep -v getem | grep -v data | grep -v progress | grep -v file

exit 0
