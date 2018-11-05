#!/usr/bin/python
#
# checkMonitors.py
#

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
os.system('ls | grep -v scripts | grep -v README | grep -v "^file" | grep -v template | grep -v known_contracts | sort -uf >/tmp/junk')
#while True:
with open('/tmp/junk') as f:
    os.system('clear')
    contracts = f.read().splitlines()
    for contract in contracts:
        command = 'cd ' + contract + ' ; cacheMan --check cache/0x*.acct.bin ; cd - >/dev/null'
        print("")
        print(command)
        os.system(command)
    f.close()
os.system('clearLocks')
