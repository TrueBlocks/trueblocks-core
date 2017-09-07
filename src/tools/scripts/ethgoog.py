#! /usr/bin/env python

# Opens Google searching for an Ethereum address but excludes well known block chains
# scrapers from the results. This makes it much easier to find references to addresses
# without getting lost in blockchain hex-hell
import webbrowser
import sys

# get the params number
params_number = len(sys.argv)


if params_number != 2:
    print("\nERROR: Invalid arguments, expected an Ethereum address as only argument\n")
    exit(1)

# Set the URL we want to open using requested address
url = 'https://www.google.com/?gws_rd=ssl#q=+' + sys.argv[1] + '+-etherscan+-ethereumblocks+-etherchain+-gastracker+-classic+-etherx+-centraltickets'

# Open it
webbrowser.open(url)
