#!/usr/bin/python
import sys
import subprocess
import os
import time
import signal
from datetime import datetime

# Signals handler, to deal with Ctrl+C stuff
def signal_handler(signal, frame):
    print('You pressed Ctrl+C!                          ')
    sys.exit(0)

# Define a signals handler to capture at least Ctrl+C
signal.signal(signal.SIGINT, signal_handler)

timeout = 5
while True:
    os.system('ls | grep -v staging | grep -v "^file" | sort -uf >./file')
    with open('./file') as f:
        contracts = f.read().splitlines()
        for contract in contracts:
            command = 'cd ' + contract + ' ; '
            command = command + 'acctScrape -l 1 -m 50000 ; '
            command = command + 'cd .. >/dev/null'
            os.system(command)
            time.sleep(0.1)
        f.close()
#        os.system('clear')
    print("{}".format(datetime.now().strftime("%Y/%m/%d:%H:%M:%S")) + ": sleeping " + str(timeout) + " seconds (Ctrl+C to quit...)")
    time.sleep(float(timeout))
