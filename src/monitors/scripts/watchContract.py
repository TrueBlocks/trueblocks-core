#!/usr/bin/python

import sys
import subprocess
import os
import time
import signal

#---------------------------------------------------------------------------------------------
# Signals handler, to deal with Ctrl+C stuff
#---------------------------------------------------------------------------------------------
def signal_handler(signal, frame):
    print('You pressed Ctrl+C!')
    sys.exit(0)

#---------------------------------------------------------------------------------------------
# Get the filesystem path for current directory (where we are currently executing the script)
#---------------------------------------------------------------------------------------------
def getmount(path):
    path = os.path.realpath(os.path.abspath(path))
    while path != os.path.sep:
        if os.path.ismount(path):
            return path
        path = os.path.abspath(os.path.join(path, os.pardir))
        return path


#---------------------------------------------------------------------------------------------
# Watch a given contract. This script needs to know the following paths:
# 1) Where are the contracts directories (contracts_path)
# 2) Where is the bin under each contract (contracts_bin_subpath)
# These paths are configured at CONFIGURATION section
#---------------------------------------------------------------------------------------------
def watchcontract(contract):
    oldpath = os.getcwd()
    newpath = contracts_path + '/' + contract 
    os.chdir(newpath)
    contract_cmd = contracts_bin_subpath + '/' + contract + ' --freshen -v'
    #print("Running command {0} at path {1}".format(contract_cmd, os.getcwd())) # debugging
    os.system(contract_cmd)
    # return to the original path
    os.chdir(oldpath)
    print(" ")

#---------------------------------------------------------------------------------------------
# Run watcher commands
#---------------------------------------------------------------------------------------------
def runwatcher():
    # Clear the screen
    os.system('clear')

    # Run in order the expected commands
    os.system('blockScrape -fl')
    os.system('blockAcct  variable')
    os.system('miniBlocks  freshen')

    # Create the vector of contracts we want to watch
#    contracts = [ 'augur', 'bnt', 'dao', 'etherTip', 'ENS', 'firstBlood', 'gnosis', 'paritySMS', 'singular', 'tenX', 'whiteHat', 'tierion']
    contracts = [ 'accts', 'augur', 'bnt', 'dao', 'etherTip', 'ENS', 'firstBlood', 'gnosis', 'paritySMS', 'singular', 'tenX', 'whiteHat', 'tierion']
    for contract in contracts:
        watchcontract(contract)

#---------------------------------------------------------------------------------------------
# Checks if parity is already running. Returns true/false
#---------------------------------------------------------------------------------------------
def is_parity_running():
    s = subprocess.check_output('ps axco command', shell=True)
    # print s # debugging
    if parity_proc_name in s:
        return True
    else:
        # Not running
        return False


#---------------------------------------------------------------------------------------------
# Gets the PID of the current parity process running at the system
#---------------------------------------------------------------------------------------------
def get_parity_pid():
    #pid = os.system('pidof %s' % parity_proc_name)
    # Get PID from name, this is only valid under Linux like environments
    pid = subprocess.check_output("pgrep " + parity_proc_name, shell=True)
    #print(pid)
    return pid

#---------------------------------------------------------------------------------------------
#  Main program
#---------------------------------------------------------------------------------------------

#---------------------
# CONFIGURATION
#---------------------
# Guard timeout while parity is starting
startup_timeout = 5
# Threshold to restart parity (When space left percentage is less than this value)
percentage_threshold = 10
# Parity process name
parity_proc_name = 'parity'
# Contracts and bin paths (can be absolute or relative path, by default using relative one present at bash script)
contracts_path = '../monitors'
contracts_bin_subpath = 'bin'

#---------------------
# EXECUTION
#---------------------

# Define a signals handler to capture at least Ctrl+C
signal.signal(signal.SIGINT, signal_handler)

# Check input parameters number
if len(sys.argv) != 2:
    print("\nERROR: Invalid arguments, expected a timeout value(seconds) as parameter\nUSAGE: {0} <timeout>\n".format(sys.argv[0]))
    exit(1)

# Validate that timeout is a positive number
timeout = sys.argv[1]
if timeout.isdigit():
    if int(timeout) <= 0:
        print("\nERROR: Invalid timeout value {0}. Expected a positive integer value\nUSAGE: {1} <timeout>\n".format(timeout, sys.argv[0]))
        exit(2)
else:
     print("\nERROR: Invalid timeout value {0}. Expected a positive integer value\nUSAGE: {1} <timeout>\n".format(timeout, sys.argv[0]))
     exit(2)

while True:
    # Start parity if it is NOT running, otherwise start watching logic directly (parity is alread running)
    if(is_parity_running() != True):
        print("Starting Parity...")
        proc = subprocess.Popen([parity_proc_name, '--tracing', 'on', '--pruning', 'archive'])
        print("Parity running with pid {0}, waiting {1} seconds before running watcher".format(proc.pid, startup_timeout))
        time.sleep(startup_timeout)
    else:
        print("Parity is already running")

    while True:
        # Get the occupation percentage at current filesystem (in GB)
        s = os.statvfs(getmount('.'))
        gigs_left = (s.f_bavail * s.f_frsize) / 1024 / 1024 / 1024
        gigs_total_space = (s.f_blocks * s.f_frsize) / 1024 / 1024 / 1024
        percentage_left = (gigs_left * 100/gigs_total_space)
        # When we have less than 10% left - restart parity
        if percentage_left < percentage_threshold:
            break
        else:
            # we can run the watcher
            runwatcher()
            time.sleep(float(timeout))
    print("Restarting Parity...")
    #os.kill(proc.pid, signal.SIGINT)
    # Get PID based on process name, this way we can also kill it when already running
    os.kill(get_parity_pid(), signal.SIGINT) #Always running at this point
    proc.wait()
