# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# folder project
project (tools)

# Path to library include files
include_directories("${CMAKE_SOURCE_DIR}/libs/include")

# messaging
# message (WARNING  "*** Entering tests folder ***")

# Compilation order of the src subdirectories
add_subdirectory(abiTest)
#add_subdirectory(acctTree)
add_subdirectory(bytesUsed)
add_subdirectory(jsonTest)
add_subdirectory(sha3)
add_subdirectory(speedTest)
add_subdirectory(stringTest)
add_subdirectory(tomlTest)
