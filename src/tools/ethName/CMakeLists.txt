# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (ethName)

# c++ compilation flags used everywhere
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O2")

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "ethName")
add_executable(${TOOL_NAME} ${SOURCE_FILES})

# Add the project static libs at linking
target_link_libraries (${TOOL_NAME} wallet token ${BASE_LIBS})

#
# TEST SECTION 
#

# Define general stuff: python script to use, paths, ...
set(TEST_SCRIPT "${TESTCASE_PATH}/scripts/run_and_compare.py")
set(TEST_EXE "${EXECUTABLE_OUTPUT_PATH}/${TOOL_NAME}")
set(TEST_INPUT_FILE_PATH "${TESTCASE_PATH}/tools/${TOOL_NAME}/tests/")
set(TEST_GOLD_FILE_PATH "${TESTCASE_GOLD_PATH}/tools/${TOOL_NAME}/tests/")

#
# Create a function (run_test) to execute a single test passing the parameters required
#  We receive the following parameters
#  - testName : Name we want to assign to the test
#  - testFileName: File name we will use in our comparison (it contains the output obtained)
#  - testArgs: The args we need to pass to the test program at command line (i.e. 0x1)
#
function(run_test testName testFileName testArgs)
	set(TEST_INPUT_FILE "${TEST_INPUT_FILE_PATH}/${testFileName}")
	set(TEST_GOLD_FILE "${TEST_GOLD_FILE_PATH}/${testFileName}")
	set(TEST_PARAMS "${TEST_EXE} ${testArgs} ${TEST_INPUT_FILE} ${TESTCASE_GOLD_FILE}")
	add_test(NAME ${testName} COMMAND ${TEST_SCRIPT} ${TEST_PARAMS})
endfunction(run_test)

# Set the params for each test and define the test case using the shared function
# TEST1 .- Here some description
run_test("ethName_test_1" "test1.txt" "0x1")

#TEST2 - Here some description
run_test("ethName_test_2" "test2.txt" "0x1 rob")
