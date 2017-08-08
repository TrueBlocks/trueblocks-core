#!/usr/bin/env python

#########################################################################################################################################
# This script receives as input the exact command we have to execute and its parameters. 
# Next-to-last parameter is the output file
# Lat parameter is the gold file we need to compare with
# (Both files can be relative or absolute paths)
#
# Example: run_anc_compare.py ethName 0x1 test/tools/tests/test1.txt test-gold/tools/tests/test1.txt 
#
# The parameters are (N: total number):
# argv[0] the name of the python script
# argv[1] the name of the command we want to run (absolute path encouraged)
# argv[2] -> argv[N-3] are the parameters passed to the command
# argv[N-2] is the output file where result is stored (this one will be compared with the gold copy at the same path but under test_gold)
# argv[N-1] is the gold copy we want to compare with
#
#########################################################################################################################################

from __future__ import print_function
import os
import sys
import subprocess
import filecmp

#-------------------------------------------------------
# Print to standard error
#-------------------------------------------------------
def printe(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

#-------------------------------------------------------
# Function to compare 2 txt files
# We receive both files as input param
#-------------------------------------------------------
def cmp_files(a, b):
        return filecmp.cmp(a, b)

#-------------------------------------------------------
# Main program
#-------------------------------------------------------

# Debugging input array
# printe(sys.argv)

# Check input parameters number
param_number = len(sys.argv)

# We need at least the python script [0], the command to be executed [1] the output file [2] and the gold file [3]
if param_number <= 3:
    printe("ERROR: Invalid parameters number, at least 3 expected")
    exit(1)

# Get the output/gold files
output_file = sys.argv[param_number-2]
gold_file = sys.argv[param_number-1]

# Check that gold file is present
if os.path.isfile(gold_file) == False:
    printe("ERROR: Could not find gold file %s" % gold_file)
    exit(2)

#Debug
#printe("%d parameters received, output file is %s gold file %s" % (param_number, output_file, gold_file))

# Build the exact command we want to run as list of items, pick the first ones from argv
# Then add the output file and redirections
command = sys.argv[1:-2]

# Debug only
# printe(command)

# Open output file and execute the command with redirections
with open(output_file, 'w') as f:
    os.chdir(os.path.dirname(output_file))
#    printe(os.getcwd())
    os.environ["NO_COLOR"] = "true"
    result = subprocess.call(command, stdout=f, stderr=subprocess.STDOUT)

if result:
    printe("ERROR: Command execution failed with error %d" % result)
    exit(result)

# Command result is OK - compare output file with gold one
if cmp_files(output_file, gold_file):
    exit(0)
else:
    printe("ERROR: Differences found comparing %s with %s" % (output_file, gold_file)) 
    exit(3)

