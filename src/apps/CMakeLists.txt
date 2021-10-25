# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# folder project
project (apps)

# Path to library include files
include_directories(${BASE_INCLUDE})

# messaging
# message (WARNING  "*** Entering apps folder ***")

# Compilation order of the src subdirectories
add_subdirectory(blockScrape)
add_subdirectory(acctExport)
add_subdirectory(cacheStatus)
add_subdirectory(chifra)
add_subdirectory(pinMan)
add_subdirectory(chunkMan)
