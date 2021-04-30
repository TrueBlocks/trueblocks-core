# ---------------------------------------------------------------------------------------
# This file belongs to the QuickBlocks project from QuickBlocks.io
# ---------------------------------------------------------------------------------------
# Minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# ----------------------------------------------------------------------------------------
# C++17 required
if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
elseif (CMAKE_CXX_STANDARD LESS 17)
    message(WARNING "CMAKE_CXX_STANDARD has been set to '${CMAKE_CXX_STANDARD}' which is lower than the minimum required standard (c++17).")
endif ()
message(STATUS "Using c++ standard c++${CMAKE_CXX_STANDARD}")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----------------------------------------------------------------------------------------
# folder project
project (trueBlocks)

# ----------------------------------------------------------------------------------------
# Debug msg
# message(DEBUG  "Building src")

# ----------------------------------------------------------------------------------------
# Allow targets in cascade
set_property (GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS 1)

# ----------------------------------------------------------------------------------------
# Mandatory python interpret, the directive PythonInterp sets the following variables
message (STATUS "======== LOOKING FOR PYTHON ========================")
# PYTHONINTERP_FOUND         - Was the Python executable found
# PYTHON_EXECUTABLE          - path to the Python interpreter
find_package (PythonInterp REQUIRED)
find_package (PythonLibs REQUIRED)

# ----------------------------------------------------------------------------------------
# We require the CURL library to be installed for most apps and tools
# if found add its include path to includes section and link with it
#
message (STATUS "======== LOOKING FOR CURL ========================")
find_package (CURL REQUIRED)
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
endif (CURL_FOUND)

# ----------------------------------------------------------------------------------------
# Globally available C++ settings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Werror -Wall -O2 -fPIC")
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Werror -Wconversion -Wall -O2 -fPIC")
endif()

# ----------------------------------------------------------------------------------------
# Linkable libraries
set (BASE_LIBS pin acct ether util ${CURL_LIBRARIES})

# ----------------------------------------------------------------------------------------
# Base included paths
set (BASE_INCLUDE 
     "${CMAKE_SOURCE_DIR}/libs/utillib"
     "${CMAKE_SOURCE_DIR}/libs/etherlib"
     "${CMAKE_SOURCE_DIR}/libs/acctlib"
     "${CMAKE_SOURCE_DIR}/libs/pinlib")

# ----------------------------------------------------------------------------------------
# Enable testing
# enable_testing()

add_custom_target(lint COMMAND "${CMAKE_SOURCE_DIR}/../bin/makeClass" "--lint" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_custom_target(format COMMAND "${CMAKE_SOURCE_DIR}/../bin/makeClass" "--format" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_custom_target(options COMMAND "${CMAKE_SOURCE_DIR}/../bin/makeClass" "--options --api" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_custom_target(generate COMMAND "${CMAKE_SOURCE_DIR}/../bin/makeClass" "-aopAm" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_custom_target(readmes COMMAND "${CMAKE_SOURCE_DIR}/../bin/makeClass" "--readmes" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_custom_target(finish COMMAND "${CMAKE_SOURCE_DIR}/../bin/makeClass" "-lf" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_custom_target(tests COMMAND "${CMAKE_SOURCE_DIR}/../bin/test/test.sh" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../build")
add_custom_target(test-all COMMAND "${CMAKE_SOURCE_DIR}/../bin/test/test-all.sh" "--no_quit" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../build")

# ----------------------------------------------------------------------------------------
# Compilation order for the subfolders
add_subdirectory (libs)
add_subdirectory (tools)
add_subdirectory (apps)
add_subdirectory (go-apps)
add_subdirectory (dev_tools)
add_subdirectory (examples)
add_subdirectory (other)
