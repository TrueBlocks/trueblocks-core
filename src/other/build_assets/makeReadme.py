#!/usr/bin/env python

#########################################################################################################################################
# This script receives parameters to do the replacement of a given tag within a text file
#
# USAGE: makeReadme.py README.tmpl.md README.footer.md toolName usageFile README.md
#
# argv[0] This script
# argv[1] The root of the source tree
# argv[1] The source code folder
# argv[2] The tool name
# argv[3] The chifra route
# argv[4] The project directory
#
#########################################################################################################################################

from __future__ import print_function
import os
import sys

# -------------------------------------------------------
# Print to standard error
# -------------------------------------------------------

def printe(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

# -------------------------------------------------------
# Main program
# -------------------------------------------------------

# Debugging input array
# printe(sys.argv)

# Check input parameters number
param_number = len(sys.argv)

# Need at least four parameters
if param_number < 4:
    printe("ERROR: Invalid parameters number, 4 expected, got ", param_number)
    exit(1)

# Get the input parameters once checked that they are present
srcFolder = sys.argv[1]
toolFolder = sys.argv[2]
toolName = sys.argv[3]
routeStr = "chifra"
if param_number > 3:
    routeStr = "chifra " + sys.argv[4]
projName = ""
if param_number > 4:
    projName = sys.argv[5]
    
templateFile = toolFolder + "/docs/README.tmpl.md"
footerFile = srcFolder + "/other/docs/README.footer.md"
usageFile = "help.txt"
outputFile = toolFolder + "/README.md"

#printe('templateFile: ', templateFile)
#printe('footerFile: ', footerFile)
#printe('toolName: ', toolName)
#printe('usageFile: ', usageFile)
#printe('outputFile: ', outputFile)
#printe('toolName: ', toolName)
#printe('srcFolder: ', srcFolder)
#printe('toolFolder: ', toolFolder)

# Check that input file is present
if os.path.isfile(templateFile) == False:
    printe("ERROR: Could not find input file %s" % templateFile)
    exit(1)

# Check that footer file is present
if os.path.isfile(footerFile) == False:
    printe("ERROR: Could not find input file %s" % footerFile)
    exit(1)

# Check that content file is present
if os.path.isfile(usageFile) == False:
    printe("ERROR: Could not find content file %s" % usageFile)
    exit(1)

# Check that output file is NOT present, remove it otherwise
if os.path.isfile(outputFile) == True:
    #printe("WARNING: Output file already present %s (removed)" % outputFile)
    os.remove(outputFile)

if os.path.isfile(outputFile) == True:
    printe("Weird")

# Get some data to use
with open(templateFile, 'r') as file:
    templateData = file.read()

with open(footerFile, 'r') as file:
    footerData = file.read()

with open(usageFile, 'r') as file:
    usageData = file.read()

# Do the replacements
outputData = templateData.replace("[{USAGE_TABLE}]", usageData)
outputData = outputData.replace("[{FOOTER}]", footerData)
outputData = outputData.replace("[{NAME}]", routeStr)
outputData = outputData.replace("[{TOOL_PATH}]", projName + "/" + toolName)
outputData = outputData.replace("chifra chifra", "chifra")

#printe('outputData: ', outputData)

# Generate output file
with open(outputFile, 'w') as file:
    printe("Wrote file %s" % outputFile)
    file.write(outputData)
