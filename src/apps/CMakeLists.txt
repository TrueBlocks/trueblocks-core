# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# folder project
project (apps)

# Path to library include files
include_directories("${CMAKE_SOURCE_DIR}/libs/include")

# messaging
# message (WARNING  "*** Entering apps folder ***")

# Compilation order of the src subdirectories
#add_subdirectory(blockScrape)
#add_subdirectory(blockAcct)
#add_subdirectory(chifra)
add_subdirectory(ethprice)
add_subdirectory(ethslurp)
add_subdirectory(grabABI)
add_subdirectory(makeClass)
#add_subdirectory(miniBlocks)
