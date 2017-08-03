# ----------------------------------------------------------------------------------------
# This file belongs to the QuickBlocks project from QuickBlocks.io
# ----------------------------------------------------------------------------------------
# Minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# ----------------------------------------------------------------------------------------
# Folder project
project (quickBlocks)

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
find_package (CURL REQUIRED)
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
endif (CURL_FOUND)

# ----------------------------------------------------------------------------------------
# Linkable libraries
set (BASE_LIBS acct abi ether util ${CURL_LIBRARIES})

# ----------------------------------------------------------------------------------------
# Folders for various purposes
set (SCRIPTS_DEST_PATH "${CMAKE_SOURCE_DIR}/../bin")

# ----------------------------------------------------------------------------------------
# Enable testing
enable_testing()

set (TEST_PATH "${CMAKE_SOURCE_DIR}/../test")
set (GOLD_PATH "${CMAKE_SOURCE_DIR}/../test-gold")
set (TEST_CHECK "${SCRIPTS_DEST_PATH}/testCheck.py")

# ----------------------------------------------------------------------------------------
# Runs test and checks output against expected gold file
function(run_the_test testPath goldPath testName testExe testArgs)
	set(TEST_FILE "${testPath}/${testName}.txt")
	set(GOLD_FILE "${goldPath}/${testName}.txt")
	add_test(NAME ${testName} COMMAND ${TEST_CHECK} ${testExe} ${testArgs} ${TEST_FILE} ${GOLD_FILE})
#	message(STATUS ${testName} " " COMMAND " " ${TEST_CHECK} " " ${testExe} " " ${testArgs} " " ${TEST_FILE} " " ${GOLD_FILE})
endfunction(run_the_test)

# ----------------------------------------------------------------------------------------
# Compilation order for the subfolders
add_subdirectory (libs)
add_subdirectory (apps)
add_subdirectory (tools)
#add_subdirectory (monitors)
#add_subdirectory (other)
