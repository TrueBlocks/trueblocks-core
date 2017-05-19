# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

project(tools)

# Compilation order of the src subdirectories
add_subdirectory(ethName)
add_subdirectory(getBalance)
add_subdirectory(getBlock)
add_subdirectory(getLogs)
add_subdirectory(getTrans)
add_subdirectory(isContract)
add_subdirectory(whenBlock)
add_subdirectory(scripts)

# Debug msg
# message(WARNING  "Building tools")


