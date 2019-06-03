#!/usr/bin/env python

#########################################################################################################################################
# This script receives as input the name of an Ethereum address and deletes its information from QBlocks cache
#########################################################################################################################################

from __future__ import print_function
import os
import sys
import errno

#-------------------------------------------------------
# Print to standard error
#-------------------------------------------------------
def printe(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

#-------------------------------------------------------
# Delete a file if it exists
#-------------------------------------------------------
def delete_file(file):
    try:
        os.remove(file)
    except OSError as e:
        # Ignore when file is not present, we skip it silently
        if(e.errno != errno.ENOENT):
            printe("ERROR: %s for file %s" % (e.strerror, file))
            exit(2)

#-------------------------------------------------------
# Displays usage at stderr
#-------------------------------------------------------
def usage():
    printe("\nUSAGE: %s <address>\n" % sys.argv[0])
    exit(1)


#-------------------------------------------------------
# Main program
#-------------------------------------------------------

# Check input parameter (we need to receive the address name)
if len(sys.argv) != 2:
    printe("\nERROR: Invalid parameters number")
    usage()

# Keep track of the input address
addr = sys.argv[1]

# Define the path where we expect the QBlocks cache files
#
# NOTE: THIS IS BROKEN SINCE WE MOVED ./slurps AND ./abis INTO
#       THE BINARY CACHES. FOR THIS TO WORK IT WOULD HAVE TO READ
#       THE TOML FILE AND USE BINARY CACHE PATH TO FIND THESE FOLDERS
#
QBLOCKS_DIR_NAME = '.quickBlocks'
SLURP_DIR_NAME = 'slurps'
ABIS_DIR_NAME = 'abis'

home = os.environ['HOME']

# Build cache files path for input address
slurp_cache = home + '/' + QBLOCKS_DIR_NAME + '/' + SLURP_DIR_NAME + '/' + addr + '.bin' 
abi_cache = home + '/' + QBLOCKS_DIR_NAME + '/' + ABIS_DIR_NAME + '/' + addr + '.abi'
json_cache = home + '/' + QBLOCKS_DIR_NAME + '/' + ABIS_DIR_NAME + '/' + addr + '.json'

# Delete them
delete_file(slurp_cache)
delete_file(abi_cache)
delete_file(json_cache)

# OK
print("Cache files for [%s] succesfully deleted" % addr)
