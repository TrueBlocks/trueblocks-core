# ---------------------------------------------------------------------------------------
# This file belongs to the QuickBlocks project from QuickBlocks.io
# ---------------------------------------------------------------------------------------
# Minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# ----------------------------------------------------------------------------------------
# Define default installation PATH
if(NOT WIN32)
    set(CMAKE_INSTALL_PREFIX "/usr/local/qblocks/" CACHE PATH "Default install directory")
else()
    message(ERROR "Windows build is not supported yet")
endif()

# ----------------------------------------------------------------------------------------
# folder project
project (quickBlocks)

# ----------------------------------------------------------------------------------------
# Debug msg
# message(DEBUG  "Building src")

# ----------------------------------------------------------------------------------------
# Add support for building some go programs
include(other/build_assets/GolangSimple.cmake)

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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror -Wall -O2 -fPIC")
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror -Wconversion -Wall -O2 -fPIC")
endif()

# ----------------------------------------------------------------------------------------
# Linkable libraries
set (BASE_LIBS acct ether util pin ${CURL_LIBRARIES})
# set (BASE_LIBS acct ether util pin sqlite3 ${CURL_LIBRARIES})

# ----------------------------------------------------------------------------------------
# Base included paths
set (BASE_INCLUDE 
     "${CMAKE_SOURCE_DIR}/libs/utillib"
     "${CMAKE_SOURCE_DIR}/libs/etherlib"
     "${CMAKE_SOURCE_DIR}/libs/acctlib"
     "${CMAKE_SOURCE_DIR}/libs/pinlib")

#set (BASE_INCLUDE 
#     "${CMAKE_SOURCE_DIR}/libs/utillib"
#     "${CMAKE_SOURCE_DIR}/libs/etherlib"
#     "${CMAKE_SOURCE_DIR}/libs/acctlib"
#     "${CMAKE_SOURCE_DIR}/libs/pinlib"
#     "${CMAKE_SOURCE_DIR}/libs/sqllit3lib")

# ----------------------------------------------------------------------------------------
# Enable testing
enable_testing()

# ----------------------------------------------------------------------------------------
# Builds the README.md file in the given folder
function(build_readme sourcePath toolName)
    add_custom_command(OUTPUT help.txt COMMAND ${toolName} "-th" ">" help.txt "2>&1")
    add_custom_target(readme COMMAND "${CMAKE_SOURCE_DIR}/../bin/test/makeReadme.py" "${CMAKE_SOURCE_DIR}" "${sourcePath}" "${toolName}" DEPENDS help.txt)
endfunction(build_readme)

# ----------------------------------------------------------------------------------------
# Compilation order for the subfolders
add_subdirectory (libs)
add_subdirectory (tools)
add_subdirectory (apps)
add_subdirectory (go-apps)
add_subdirectory (other)
add_subdirectory (dev_tools)
