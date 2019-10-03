# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# folder project
project (dev_tools)

# messaging
# message (WARNING  "*** Entering other folder ***")

# Compilation order of the src subdirectories
add_subdirectory(makeClass) # do this first
add_subdirectory(testRunner)
