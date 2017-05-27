# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

project(apps)

# Compilation order of the src subdirectories
#add_subdirectory(blockScrape)
#add_subdirectory(chifra)
add_subdirectory(ethprice)
add_subdirectory(ethslurp)
<<<<<<< HEAD
add_subdirectory(ethprice)
=======
add_subdirectory(grabABI)
add_subdirectory(makeClass)
#add_subdirectory(miniBlocks)
>>>>>>> cmake-branch

# Debug msg
#message(WARNING  "Building apps")
