#!/usr/bin/env python

import os

os.system('echo "---------------------------------------------------------------------" | tee tests/results.txt')
os.system('echo "no input" | tee -a tests/results.txt')
os.system('getTrans 2>&1 | tee -a tests/results.txt')

os.system('echo "---------------------------------------------------------------------" | tee -a tests/results.txt')
os.system('echo "bad hash" | tee -a tests/results.txt')
os.system('getTrans 0x0012 2>&1 | tee -a tests/results.txt')

os.system('echo "---------------------------------------------------------------------" | tee -a tests/results.txt')
os.system('echo "bad index" | tee -a tests/results.txt')
os.system('getTrans -v 0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.0 2>&1 | tee -a tests/results.txt')

os.system('echo "---------------------------------------------------------------------" | tee -a tests/results.txt')
os.system('echo "get transaction 3657480.79 three different ways" | tee -a tests/results.txt')
os.system('getTrans -v 0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.79 3657480.79 0xda2f27fffb36c90c74fc53bd8b721074e87203c727640c95c4c802296e3bf8ad 2>&1 | tee -a tests/results.txt')

os.system('echo "---------------------------------------------------------------------" | tee -a tests/results.txt')
os.system('echo "get by --file three ways" | tee -a tests/results.txt')
os.system('getTrans --file:tests/cmd_file 2>&1 | tee -a tests/results.txt')
os.system('getTrans -v --file:tests/cmd_file 2>&1 | tee -a tests/results.txt')
