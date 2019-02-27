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
    os.system('ls 0x*.acct.bin | sed "s/\.acct\.bin//" >./file')
    with open('./file') as f:
        addrs = f.read().splitlines()
        for addr in addrs:
            os.system('chifra freshen ' + addr)
            time.sleep(0.1)
        f.close()
    print("{}".format(datetime.now().strftime("%Y/%m/%d:%H:%M:%S")) + ": sleeping " + str(timeout) + " seconds (Ctrl+C to quit...)")
    time.sleep(float(timeout))
