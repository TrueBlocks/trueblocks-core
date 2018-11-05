#!/usr/bin/python
#
# oneTest.py
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

contract = ""
if len(sys.argv) > 1:
    contract = sys.argv[1]

if contract != "":
#    print(contract)
    command = 'cacheMan -d -v --maxBlock 5250000 cache/0x*.acct.bin | tee ../../test/gold/monitors/' + contract + '.txt ; cd - >/dev/null'
    os.system(command)
