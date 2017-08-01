#!/usr/bin/env python

import os
os.system('echo "should show help screen"  >tests/results.txt')
os.system('getBalance                  >>tests/results.txt 2>&1')
os.system('echo >>tests/results.txt');

os.system('echo "bad address"              >>tests/results.txt')
os.system('getBalance $dao 0x12        >>tests/results.txt 2>&1')
os.system('echo >>tests/results.txt');

os.system('echo "bad block"                >>tests/results.txt')
os.system('getBalance 12 -1            >>tests/results.txt 2>&1')
os.system('echo >>tests/results.txt');

os.system('echo "no address"               >>tests/results.txt')
os.system('getBalance 12               >>tests/results.txt 2>&1')
os.system('echo >>tests/results.txt');

os.system('echo "no block (should default to latest block)" >>tests/results.txt')
os.system('echo "Not run" >>tests/results.txt 2>&1')
#os.system('getBalance 0xbb9bc244d798123fde783fcc1c72d3bb8c189413  >>tests/results.txt 2>&1')
os.system('echo >>tests/results.txt');

os.system('echo "get balance of dao at block 1500000" >>tests/results.txt')
os.system('getBalance 0xbb9bc244d798123fde783fcc1c72d3bb8c189413 1500000  >>tests/results.txt 2>&1')
os.system('echo >>tests/results.txt');

os.system('echo "get balance of two accounts at 1500000" >>tests/results.txt')
os.system('getBalance 0xbb9bc244d798123fde783fcc1c72d3bb8c189413 $tip 1500000  >>tests/results.txt 2>&1')
os.system('echo >>tests/results.txt');

os.system('echo "get balance of tip jar at four different blocks" >>tests/results.txt')
os.system('getBalance 0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359 1500000 2000000 2500000 3000000 >>tests/results.txt 2>&1')
os.system('echo >>tests/results.txt');

os.system('echo "same tests with ether" >>tests/results.txt')
os.system('echo >>tests/results.txt');

os.system('echo "get balance of dao at block 1500000 in ether" >>tests/results.txt')
os.system('getBalance -e 0xbb9bc244d798123fde783fcc1c72d3bb8c189413 1500000  >>tests/results.txt 2>&1')
os.system('echo >>tests/results.txt');

os.system('echo "get balance of two accounts at 1500000 in ether" >>tests/results.txt')
os.system('getBalance -e 0xbb9bc244d798123fde783fcc1c72d3bb8c189413 0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359 1500000 >>tests/results.txt 2>&1')
os.system('echo >>tests/results.txt');

os.system('echo "get balance of tip jar at four different blocks in ether" >>tests/results.txt')
os.system('getBalance -e 0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359 1500000 2000000 2500000 3000000 >>tests/results.txt 2>&1')
os.system('echo >>tests/results.txt');

os.system('echo "from file" >>tests/results.txt')
os.system('getBalance --file:tests/cmd_file >>tests/results.txt 2>&1')
os.system('echo >>tests/results.txt');
