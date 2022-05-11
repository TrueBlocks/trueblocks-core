# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# folder project
project (examples)

# Path to library include files
include_directories(${BASE_INCLUDE})

# messaging
# message (WARNING  "*** Entering examples folder ***")

# Compilation order of the src subdirectories
add_subdirectory(simple)
add_subdirectory(findFirst)
add_subdirectory(forEvery)
add_subdirectory(forEveryIndex)
add_subdirectory(txCounter)
