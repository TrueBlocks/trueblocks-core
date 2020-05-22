# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# folder project
project (test-libs)

# Path to library include files
include_directories(${BASE_INCLUDE})

# messaging
# message (WARNING  "*** Entering libs folder ***")

# Compile the libraries in this order
add_subdirectory(utillib)
add_subdirectory(etherlib)
add_subdirectory(acctlib)
add_subdirectory(pinlib)
#add_subdirectory(sqlite3lib)
