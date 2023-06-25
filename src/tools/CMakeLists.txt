# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# folder project
project (tools)

# Path to library include files
include_directories(${BASE_INCLUDE})

# messaging
# message (WARNING  "*** Entering tools folder ***")

# Compilation order of the src subdirectories
add_subdirectory(getBlocks)
add_subdirectory(getTokens)
