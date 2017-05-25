# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

project(apps)

# Compilation order of the src subdirectories
add_subdirectory(ethslurp)
add_subdirectory(ethprice)

# Debug msg
#message(WARNING  "Building apps")

