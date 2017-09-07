#! /usr/bin/env python
#
# Usage: ethscan [ tx hash | address | block # ]
# Opens http://etherscan.io with either a transaction hash, an Ethereum address or a block number

import sys
import webbrowser

# Set the default URL we want to open
url = 'http://etherscan.io'

# get the params number
params_number = len(sys.argv)

#-------------------------------------------------------
# Open a given URL passed as argument
#-------------------------------------------------------
def open_url(url):
    #print("Opening URL " + url) # debug
    webbrowser.open(url)
    
#-------------------------------------------------------
# Main program
# Checks the input parameters and their length to determine 
# the URL to be launched
#-------------------------------------------------------

if params_number > 1:
    if params_number == 2:
        param_len = len(sys.argv[1])

        # Debug
        #print("Len: %d" % param_len)

        if param_len == 66:
	    open_url(url + '/tx/' + sys.argv[1])
        elif param_len == 42:
            open_url(url + '/address/' + sys.argv[1])
        else:
            open_url(url + '/block/' + sys.argv[1])
    else:
        # More than one parameter passed
	print("\nERROR: Specify only a single transaction hash, account address, or block number\n")
else:
    # No arguments, open the URL only
    open_url(url)
