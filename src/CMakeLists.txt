# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

project(quickBlocks)

# Debug msg
# message(DEBUG  "Building src")

set_property(GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS 1)

set(README_TARGET "readme")

# Compilation order of the src subdirectories
add_subdirectory(libs)
add_subdirectory(apps)
add_subdirectory(tools)


