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

timeout = 14
if len(sys.argv) > 1:
    timeout = sys.argv[1]

option = ""
if len(sys.argv) > 2:
    option = "--" + sys.argv[2]

# We need to know where to run from
if 'QB_MONITORS' in os.environ:
    dir = os.getenv('QB_MONITORS')
    os.chdir(dir + '/monitors/')
    os.system('pwd')
else:
    print('You must set $QB_MONITORS                         ')
    sys.exit(0)

os.system('ls | grep -v CONFIG | grep -v staging | grep -v README | grep -v "^file" | grep -v final | grep -v test_cases | grep -v 0x | sort -uf >./file')
while True:
    with open('./file') as f:
        contracts = f.read().splitlines()
        for contract in contracts:
            command = 'cd ' + contract + ' ; '
            command = command + 'acctScrape -l 1 -m 50000 ' + option + ' ; '
            command = command + 'cd .. >/dev/null'
            os.system(command)
#            print(command)
            time.sleep(0.25)
        f.close()
    os.system('rm -f `clearLocks`')
    print("Hit Ctrl+C to quit...                                                                                     ");
    time.sleep(float(timeout))
