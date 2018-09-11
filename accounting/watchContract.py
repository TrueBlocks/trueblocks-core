#!/usr/bin/python

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

while True:
    os.system('clear')
    os.system('cd group_02 ; acctScrape -m 50000 ; cd - >/dev/null')
    os.system('cd group_03 ; acctScrape -m 50000 ; cd - >/dev/null')
    os.system('cd group_04 ; acctScrape -m 50000 ; cd - >/dev/null')
    print("Hit Ctrl+C to quit...                                                                                     ");
    time.sleep(float(2.))
