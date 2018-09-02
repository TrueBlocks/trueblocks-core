#!/usr/bin/python

import sys
import subprocess
import os
import time
import signal

# Signals handler, to deal with Ctrl+C stuff
def signal_handler(signal, frame):
    print('You pressed Ctrl+C!')
    sys.exit(0)

# Define a signals handler to capture at least Ctrl+C
signal.signal(signal.SIGINT, signal_handler)

os.chdir('../monitors')
os.system('ls | grep -v scripts | grep -v README | grep -v "^file" | grep -v template | grep -v known_contracts | grep -v ENS | grep -v accts | sort -uf >/tmp/junk')
with open('/tmp/junk') as f:
    os.system('clear')
    contracts = f.read().splitlines()
    for contract in contracts:
        command = 'cd ' + contract + ' ; acctExport --mysql --maxRows 25000 ; cd - >/dev/null'
        print(command)
        os.system(command)
    f.close()
os.system('clearLocks')
