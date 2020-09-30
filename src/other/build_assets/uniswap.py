#! /usr/bin/env python
#
# Usage: uniswap pair_address
# Opens http://uniswap.info with the address of the pair to display

import sys
import webbrowser

#-------------------------------------------------------
url = 'http://uniswap.info'
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
            if argLen == 42:
                open_url(url + '/pair/' + arg)
        been_here = 1
