# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

project(apps)

# Compilation order of the src subdirectories
#add_subdirectory(blockScrape)
#add_subdirectory(chifra)
add_subdirectory(ethprice)
add_subdirectory(ethslurp)
add_subdirectory(grabABI)
add_subdirectory(makeClass)
#add_subdirectory(miniBlocks)

# Debug msg
#message(WARNING  "Building apps")
