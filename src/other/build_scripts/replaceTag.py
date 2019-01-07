#!/usr/bin/env python

#########################################################################################################################################
# This script receives parameters to do the replacement of a given tag within a text file
#
# USAGE: replaceTag.py README.tmpl.md README.footer.md toolName usageFile README.md
#
# argv[0] The readme template file
# argv[1] The readme footer file
# argv[2] The name of the tool that produced the usage table
# argv[3] help.txt (generated usage table)
# argv[4] The output README.md file
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

# Need at least four parameters
if param_number <= 5:
    printe("ERROR: Invalid parameters number, at least 4 expected")
    exit(1)

# Get the input parameters once checked that they are present
templateFile = sys.argv[1]
footerFile   = sys.argv[2]
toolName     = sys.argv[3]
usageFile    = sys.argv[4]
outputFile   = sys.argv[5]

#printe(templateFile)
#printe(footerFile)
#printe(toolName)
#printe(usageFile)
#printe(outputFile)

# Check that input file is present
if os.path.isfile(templateFile) == False:
    printe("ERROR: Could not find input file %s" % templateFile)
    exit(1)

# Check that content file is present
if os.path.isfile(usageFile) == False:
    printe("ERROR: Could not find content file %s" % usageFile)
    exit(1)

# Check that output file is NOT present, remove it otherwise
if os.path.isfile(outputFile) == True:
#   printe("WARNING: Output file already present %s (removed)" % outputFile)
    os.remove(outputFile)

# Get some data to use
with open(templateFile, 'r') as file :
    templateData = file.read()

with open(footerFile, 'r') as file :
    footerData = file.read()

with open(usageFile, 'r') as file :
    usageData = file.read()

# Do the replacements
outputData = templateData.replace("[{USAGE_TABLE}]", usageData).replace("[{FOOTER}]", footerData).replace("[{NAME}]", toolName)

# Generate output file
with open(outputFile, 'w') as file:
    printe("Wrote file %s" % outputFile)
    file.write(outputData)
