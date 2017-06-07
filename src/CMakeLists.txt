# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# folder project
project (quickBlocks)

# allow targets in cascade
set_property (GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS 1)

# mandatory python interpret, the directive PythonInterp sets the following variables
#PYTHONINTERP_FOUND         - Was the Python executable found
#PYTHON_EXECUTABLE          - path to the Python interpreter

message (STATUS "======== LOOKING FOR PYTHON ========================")

find_package (PythonInterp REQUIRED)
find_package (PythonLibs REQUIRED)

# we require the CURL library to be installed for most apps and tools
# if found add its include path to includes section and link with it
find_package (CURL REQUIRED)
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
endif (CURL_FOUND)

# linkable libraries
set (BASE_LIBS util abi ether ${CURL_LIBRARIES})

# copy destination for various shell scripts
set (SCRIPTS_DEST_PATH "${CMAKE_SOURCE_DIR}/../bin")

# compilation order of the src subdirectories
add_subdirectory (libs)
add_subdirectory (apps)
add_subdirectory (tools)
add_subdirectory (tests)
#add_subdirectory (monitors)
#add_subdirectory (other)
