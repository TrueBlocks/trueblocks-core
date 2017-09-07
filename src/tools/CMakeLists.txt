# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# folder project
project (tools)

# Path to library include files
include_directories("${CMAKE_SOURCE_DIR}/libs/include")

# messaging
# message (WARNING  "*** Entering tools folder ***")

# Compilation order of the src subdirectories
add_subdirectory(ethName)
add_subdirectory(getBlock)
add_subdirectory(getBloom)
add_subdirectory(getLogs)
add_subdirectory(getReceipt)
add_subdirectory(getTrans)
add_subdirectory(getTrace)
add_subdirectory(getBalance)
add_subdirectory(getTokenBal)
add_subdirectory(isContract)
add_subdirectory(whenBlock)
add_subdirectory(whereBlock)
add_subdirectory(scripts)
