#!/usr/bin/env python

import os
os.system('ethName 0x1 >tests/test1.txt 2>&1')
os.system('ethName 0x1 rob >tests/test2.txt 2>&1')
os.system('ethName 0x1 rob steem >tests/test3.txt 2>&1')
os.system('ethName 0xbf4ed7b27f1d666546e30d74d50d173d20bca754 >tests/test4.txt 2>&1')
os.system('ethName cur >tests/test5.txt 2>&1')
os.system('ethName cur -s >tests/test6.txt 2>&1')

exit(0)
