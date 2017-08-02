# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# folder project
project (quickBlocks)

# debug msg
# message(DEBUG  "Building src")

# allow targets in cascade
set_property (GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS 1)

# mandatory python interpret, the directive PythonInterp sets the following variables
message (STATUS "======== LOOKING FOR PYTHON ========================")
#PYTHONINTERP_FOUND         - Was the Python executable found
#PYTHON_EXECUTABLE          - path to the Python interpreter
find_package (PythonInterp REQUIRED)
find_package (PythonLibs REQUIRED)

# Enable testing
enable_testing()

# we require the CURL library to be installed for most apps and tools
# if found add its include path to includes section and link with it
find_package (CURL REQUIRED)
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
endif (CURL_FOUND)

# linkable libraries
set (BASE_LIBS acct abi ether util ${CURL_LIBRARIES})

# folders for various purposes
set (SCRIPTS_DEST_PATH "${CMAKE_SOURCE_DIR}/../bin")
set (TESTCASE_PATH "${CMAKE_SOURCE_DIR}/../test")

# compilation order of the src subdirectories
add_subdirectory (libs)
add_subdirectory (apps)
add_subdirectory (tools)
#add_subdirectory (monitors)
#add_subdirectory (other)
