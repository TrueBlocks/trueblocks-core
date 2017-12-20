#! /usr/bin/env python
#
# Usage: ethscan [ tx hash | address | block # ]
# Opens http://etherscan.io with either a transaction hash, an Ethereum address or a block number

import sys
import webbrowser

#-------------------------------------------------------
url = 'http://etherscan.io'
nArgs = len(sys.argv)

#-------------------------------------------------------
def open_url(url):
    webbrowser.open(url)

#-------------------------------------------------------
if nArgs == 1:
    open_url(url) # If no args, open the main site
else:
    been_here = 0 # skip arg0
    for arg in sys.argv:
        if (been_here):
            argLen = len(arg)
            if argLen == 66:
                open_url(url + '/tx/' + arg)
            elif argLen == 42:
                open_url(url + '/address/' + arg)
            else:
                open_url(url + '/block/' + arg)
        been_here = 1
