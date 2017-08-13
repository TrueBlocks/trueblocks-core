#!/usr/bin/env python

#########################################################################################################################################
# This script receives parameters to do the replacement of a given tag within a text file
#
# USAGE: replaceTag.py input_file name content_file output_file
# argv[0] The input file where we need to replace the usage table
# argv[1] The name of the tool or app
# argv[2] The file with the text we want to use in replacement (tag -> text)
# argv[3] Name of the output file we obtain after doing the replacement
#
# Example: replaceTag.py README.tmpl.md [[TOOL_NAME]] HELP.txt README.md
#
#########################################################################################################################################

from __future__ import print_function
import os
import sys

#-------------------------------------------------------
# Print to standard error
#-------------------------------------------------------
def printe(*args, **kwargs):
        print(*args, file=sys.stderr, **kwargs)

#-------------------------------------------------------
# Main program
#-------------------------------------------------------

# Debugging input array
# printe(sys.argv)

# Check input parameters number
param_number = len(sys.argv)

# We need the following parameters:
# 1) input file,
# 2) the name of the tool/app
# 3) the content file (we replace tag with the content of this file)
# 4) the name of the output file we will generate after replacing the tag at input file
if param_number <= 4:
    printe("ERROR: Invalid parameters number, at least 4 expected")
    exit(1)

# Get the input parameters once checked that they are present
input_file = sys.argv[1]
name = sys.argv[2]
content_file = sys.argv[3]
output_file = sys.argv[4]

# Check that input file is present
if os.path.isfile(input_file) == False:
    printe("ERROR: Could not find input file %s" % input_file)
    exit(1)

# Check that content file is present
if os.path.isfile(content_file) == False:
    printe("ERROR: Could not find content file %s" % content_file)
    exit(1)

# Check that output file is NOT present, remove it otherwise
if os.path.isfile(output_file) == True:
#    printe("WARNING: Output file already present %s (removed)" % output_file)
    os.remove(output_file)

# Open input file
with open(input_file, 'r') as file :
    input_data = file.read()

# Open content file
with open(content_file, 'r') as file :
     content_data = file.read()

# Do the replacement
output_data = input_data.replace("[{COMMAND_HELP}]", content_data)
output_data = output_data.replace("[{NAME}]", name)

# Generate output file
with open(output_file, 'w') as file:
    printe("Wrote file %s" % output_file)
    file.write(output_data)
