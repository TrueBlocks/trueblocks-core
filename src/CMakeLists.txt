# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

project(quickBlocks)

# Debug msg
# message(DEBUG  "Building src")

# Allow targets in cascade
set_property(GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS 1)

# Mandatory python interpret, the directive PythonInterp sets the following variables
#PYTHONINTERP_FOUND         - Was the Python executable found
#PYTHON_EXECUTABLE          - path to the Python interpreter

message(STATUS "======== LOOKING FOR PYTHON ========================")

find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)

set(README_TARGET "readme")

# Compilation order of the src subdirectories
add_subdirectory(libs)
add_subdirectory(apps)
add_subdirectory(tools)
add_subdirectory(tests)
#add_subdirectory(monitors)
#add_subdirectory(other)
