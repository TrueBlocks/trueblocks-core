# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# folder project
project (tools)

# Path to library include files
include_directories(${BASE_INCLUDE})

# messaging
# message (WARNING  "*** Entering tools folder ***")

# Compilation order of the src subdirectories
add_subdirectory(ethNames)
add_subdirectory(getQuotes)
add_subdirectory(ethslurp)
add_subdirectory(getBlock)
add_subdirectory(getLogs)
add_subdirectory(getReceipts)
add_subdirectory(getState)
add_subdirectory(getTokens)
add_subdirectory(getTraces)
add_subdirectory(getTrans)
add_subdirectory(grabABI)
add_subdirectory(whenBlock)
