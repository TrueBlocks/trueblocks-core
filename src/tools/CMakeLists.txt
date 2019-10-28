# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# folder project
project (tools)

# Path to library include files
include_directories(${BASE_INCLUDE})

# messaging
# message (WARNING  "*** Entering tools folder ***")

# Compilation order of the src subdirectories
add_subdirectory(ethslurp)
add_subdirectory(ethQuote)
add_subdirectory(ethNames)
add_subdirectory(getBlock)
add_subdirectory(getLogs)
add_subdirectory(getReceipt)
add_subdirectory(getTrans)
add_subdirectory(getTrace)
add_subdirectory(getState)
add_subdirectory(grabABI)
add_subdirectory(whenBlock)
add_subdirectory(getTokenInfo)
add_subdirectory(getBloom)
add_subdirectory(whereBlock)
