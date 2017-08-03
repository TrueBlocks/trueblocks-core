# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (getBalance)

# c++ compilation flags used everywhere
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O2")

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "getBalance")
add_executable(${TOOL_NAME} ${SOURCE_FILES})

# Add the project static libs at linking
target_link_libraries (${TOOL_NAME} wallet token ${BASE_LIBS})

# Testing
# Define paths to test folder and gold folder
#set(TEST_EXE "${EXECUTABLE_OUTPUT_PATH}/${TOOL_NAME}")
#set(TEST_INPUT_FILE_PATH "${TESTCASE_PATH}/tools/${TOOL_NAME}/tests/")
#set(TEST_GOLD_FILE_PATH  "${TESTCASE_GOLD_PATH}/tools/${TOOL_NAME}/tests/")
#
# Function to run an individual test case
#function(run_test testName testFileName testArgs)
#	set(TEST_INPUT_FILE "${TEST_INPUT_FILE_PATH}/${testFileName}")
#	set(TEST_GOLD_FILE "${TEST_GOLD_FILE_PATH}/${testFileName}")
#	add_test(NAME ${testName} COMMAND ${TESTCASE_SCRIPT} ${TEST_EXE} ${testArgs} ${TEST_INPUT_FILE} ${TEST_GOLD_FILE})
#endfunction(run_test)
#
#run_test(“getBalance_help_screen”, “test1.txt”, “”)
#
# echo "should show help screen"  >tests/results.txt')
# getBalance                  >>tests/results.txt 2>&1')
# echo >>tests/results.txt');
#
# echo "bad address"              >>tests/results.txt')
# getBalance $dao 0x12        >>tests/results.txt 2>&1')
# echo >>tests/results.txt');
#
# echo "bad block"                >>tests/results.txt')
# getBalance 12 -1            >>tests/results.txt 2>&1')
# echo >>tests/results.txt');
#
# echo "no address"               >>tests/results.txt')
# getBalance 12               >>tests/results.txt 2>&1')
# echo >>tests/results.txt');
#
# echo "no block (should default to latest block)" >>tests/results.txt')
# echo "Not run" >>tests/results.txt 2>&1')
## getBalance 0xbb9bc244d798123fde783fcc1c72d3bb8c189413  >>tests/results.txt 2>&1')
# echo >>tests/results.txt');
#
# echo "get balance of dao at block 1500000" >>tests/results.txt')
# getBalance 0xbb9bc244d798123fde783fcc1c72d3bb8c189413 1500000  >>tests/results.txt 2>&1')
# echo >>tests/results.txt');
#
# echo "get balance of two accounts at 1500000" >>tests/results.txt')
# getBalance 0xbb9bc244d798123fde783fcc1c72d3bb8c189413 $tip 1500000  >>tests/results.txt 2>&1')
# echo >>tests/results.txt');
#
# echo "get balance of tip jar at four different blocks" >>tests/results.txt')
# getBalance 0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359 1500000 2000000 2500000 3000000 >>tests/results.txt 2>&1')
# echo >>tests/results.txt');
#
# echo "same tests with ether" >>tests/results.txt')
# echo >>tests/results.txt');
#
# echo "get balance of dao at block 1500000 in ether" >>tests/results.txt')
# getBalance -e 0xbb9bc244d798123fde783fcc1c72d3bb8c189413 1500000  >>tests/results.txt 2>&1')
# echo >>tests/results.txt');
#
# echo "get balance of two accounts at 1500000 in ether" >>tests/results.txt')
# getBalance -e 0xbb9bc244d798123fde783fcc1c72d3bb8c189413 0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359 1500000 >>tests/results.txt 2>&1')
# echo >>tests/results.txt');
#
# echo "get balance of tip jar at four different blocks in ether" >>tests/results.txt')
# getBalance -e 0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359 1500000 2000000 2500000 3000000 >>tests/results.txt 2>&1')
# echo >>tests/results.txt');
#
# echo "from file" >>tests/results.txt')
# getBalance --file:tests/cmd_file >>tests/results.txt 2>&1')
# echo >>tests/results.txt');
#
#run_test("ethName_simple_search"  "test1.txt" "0x1")
#run_test("ethName_invalid_search" "test2.txt" "0x1 rob")
