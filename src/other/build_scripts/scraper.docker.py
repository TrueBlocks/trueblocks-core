#!/usr/bin/python
#
# scraper.docker.py
#
import sys
import subprocess
import os
import time
import signal

def signal_handler(signal, frame):
    print('You pressed Ctrl+C!')
    sys.exit(0)

# Define a signals handler to capture at least Ctrl+C
signal.signal(signal.SIGINT, signal_handler)

timeout = 5
while True:
    os.system('blockScrape --silent --consolidate --addrIndex')
    os.system('getBlock --latest')
    print("")
    time.sleep(float(timeout))
