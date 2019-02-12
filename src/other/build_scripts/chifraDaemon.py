#!/usr/bin/python
#
# watchContract.py
#

import sys
import subprocess
import os
import time
import signal

# Signals handler, to deal with Ctrl+C stuff
def signal_handler(signal, frame):
    print('You pressed Ctrl+C!                          ')
    sys.exit(0)

# Define a signals handler to capture at least Ctrl+C
signal.signal(signal.SIGINT, signal_handler)

timeout = 15
while True:
    os.system('ls | grep -v staging | grep -v "^file" | sort -uf >./file')
    with open('./file') as f:
        os.system('clear')
        contracts = f.read().splitlines()
        for contract in contracts:
            command = 'cd ' + contract + ' ; '
            command = command + 'acctScrape -l 1 -m 50000 ; '
            command = command + 'cd .. >/dev/null'
            os.system(command)
#            print(command)
            time.sleep(0.1)
        f.close()
#    os.system('rm -f `clearLocks`')
    print("Hit Ctrl+C to quit...                                                                                     ");
    time.sleep(float(timeout))
