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
set (BASE_LIBS acct ether util ${CURL_LIBRARIES})

# ----------------------------------------------------------------------------------------
# Base included paths
set (BASE_INCLUDE 
     "${CMAKE_SOURCE_DIR}/libs/utillib"
     "${CMAKE_SOURCE_DIR}/libs/etherlib"
     "${CMAKE_SOURCE_DIR}/libs/acctlib")

# ----------------------------------------------------------------------------------------
# Folders for various purposes
set (SCRIPTS_DEST_PATH "${CMAKE_SOURCE_DIR}/../bin")
set (BUILD_DEST_PATH "${CMAKE_SOURCE_DIR}/../bin/test")

# ----------------------------------------------------------------------------------------
# Enable testing
enable_testing()

set (TEST_PATH   "${CMAKE_SOURCE_DIR}/../test/working")
set (GOLD_PATH   "${CMAKE_SOURCE_DIR}/../test/gold")
set (TEST_CHECK  "${SCRIPTS_DEST_PATH}/testCheck.py")
set (TAG_REPLACE "${SCRIPTS_DEST_PATH}/replaceTag.py")

# ----------------------------------------------------------------------------------------
# Builds the README.md file in the given folder
function(build_readme sourcePath toolName)
    set(README_FILE "${sourcePath}/README.md")
    set(TOOL_README_TEMPLATE "${sourcePath}/docs/README.tmpl.md")
    set(README_FOOTER "${CMAKE_SOURCE_DIR}/other/docs/README.footer.md")
    add_custom_command(OUTPUT help.txt COMMAND ${toolName} "-th" ">" help.txt "2>&1")
    add_custom_target(readme COMMAND "${TAG_REPLACE}" "${TOOL_README_TEMPLATE}" "${README_FOOTER}" "${toolName}" help.txt "${README_FILE}" DEPENDS help.txt)
endfunction(build_readme)

# ----------------------------------------------------------------------------------------
# Runs test and checks output against expected gold file
function(run_the_test testPath goldPath testName testExe) # Optional argv4 with command parameters
	set(TEST_FILE "${testPath}/${testName}.txt")
	set(GOLD_FILE "${goldPath}/${testName}.txt")
	if (NOT ${ARGV4} STREQUAL "" )
	    # In ARGV4 we receive the parameters to be passed to testExe  (optional)
	    add_test(NAME ${testName} COMMAND ${TEST_CHECK} ${testExe} ${ARGN} ${TEST_FILE} ${GOLD_FILE})
	    #message(STATUS ${TEST_CHECK} " " ${testExe} " " ${ARGN} " " ${TEST_FILE} " " ${GOLD_FILE})
    else()
	    add_test(NAME ${testName} COMMAND ${TEST_CHECK} ${testExe} ${TEST_FILE} ${GOLD_FILE})
	    #message(STATUS ${TEST_CHECK} " " ${testExe} " " ${TEST_FILE} " " ${GOLD_FILE})
    endif()
endfunction(run_the_test)

# ----------------------------------------------------------------------------------------
# Runs specials tests that are only run with "make test-all"
function(run_the_special_test testPath goldPath testName testExe) # Optional argv4 with command parameters
    set(TEST_FILE "${testPath}/${testName}.txt")
    set(GOLD_FILE "${goldPath}/${testName}.txt")
    if (NOT ${ARGV4} STREQUAL "" )
        # In ARGV4 we receive the parameters to be passed to testExe  (optional)
        add_test(NAME ${testName} COMMAND ${TEST_CHECK} ${testExe} ${ARGN} ${TEST_FILE} ${GOLD_FILE} CONFIGURATIONS test_all)
	    #message(STATUS ${TEST_CHECK} " " ${testExe} " " ${ARGN} " " ${TEST_FILE} " " ${GOLD_FILE})
    else()
        add_test(NAME ${testName} COMMAND ${TEST_CHECK} ${testExe} ${TEST_FILE} ${GOLD_FILE} CONFIGURATIONS test_all)
	    #message(STATUS ${TEST_CHECK} " " ${testExe} " " ${TEST_FILE} " " ${GOLD_FILE})
    endif()
endfunction(run_the_special_test)

# ----------------------------------------------------------------------------------------
# Runs state tests that are only run with "make test-state"
function(run_the_state_test testPath goldPath testName testExe) # Optional argv4 with command parameters
    set(TEST_FILE "${testPath}/${testName}.txt")
    set(GOLD_FILE "${goldPath}/${testName}.txt")
    if (NOT ${ARGV4} STREQUAL "" )
        # In ARGV4 we receive the parameters to be passed to testExe  (optional)
        add_test(NAME ${testName} COMMAND ${TEST_CHECK} ${testExe} ${ARGN} ${TEST_FILE} ${GOLD_FILE} CONFIGURATIONS test_state)
	    #message(STATUS ${TEST_CHECK} " " ${testExe} " " ${ARGN} " " ${TEST_FILE} " " ${GOLD_FILE})
    else()
        add_test(NAME ${testName} COMMAND ${TEST_CHECK} ${testExe} ${TEST_FILE} ${GOLD_FILE} CONFIGURATIONS test_state)
	    #message(STATUS ${TEST_CHECK} " " ${testExe} " " ${TEST_FILE} " " ${GOLD_FILE})
    endif()
endfunction(run_the_state_test)

# ----------------------------------------------------------------------------------------
## Function to Concatenate multiple files in a single output file
## Rename or move to root CMakeLists.txt if you cosider it handy
## Parameters :
## input_list - input files (list)
## output_file - output file
#function(concatenate_files input_list output_file)
#    file(WRITE ${OUT} "")
#    foreach(FILENAME ${input_list})
#        file(READ ${FILENAME} CONTENTS)
#    file(APPEND ${output_file} "${CONTENTS}")
#    endforeach()
#endfunction(concatenate_files)
## We need to generate for test 01 the input files blocks.txt and dates.txt. How?
## NOTE FROM JAY: I PUT THESE FILES IN THE GOLD FOLDER AND CHANGE CWD TO GOLD TO RUN THE TEST
## Group them in a list
##set(INPUT_LIST "/path/to/blocks.txt" "/path/To/dates.txt")
## Once we have them we can concatenate both files using the function concatenate_files
## set(OUTPUT_TEST_FILE "/path/to/results.txt")
##add_custom_command(OUTPUT ${OUTPUT_TEST_FILE} COMMAND concatenate_files ${INPUT_LIST} ${OUTPUT_TEST_FILE})

# ----------------------------------------------------------------------------------------
# Compilation order for the subfolders
add_subdirectory (libs)
add_subdirectory (tools)
add_subdirectory (apps)
add_subdirectory (go-apps)
add_subdirectory (other)
add_subdirectory (dev_tools)
