# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (getTokenBal)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "getTokenBal")
set(PROJ_NAME "tools")
add_executable(${TOOL_NAME} ${SOURCE_FILES})

# Add the project static libs at linking
target_link_libraries (${TOOL_NAME} wallet token ${BASE_LIBS})

# Testing
# Define paths to test folder and gold folder
set(TEST_EXE "${EXECUTABLE_OUTPUT_PATH}/${TOOL_NAME}")
set(TEST_PATH "${TEST_PATH}/${PROJ_NAME}/${TOOL_NAME}")
set(GOLD_PATH "${GOLD_PATH}/${PROJ_NAME}/${TOOL_NAME}")

# Additional target to make the README.md
build_readme(${CMAKE_CURRENT_SOURCE_DIR} ${TOOL_NAME})

# Function to run an special or slow test case
function(run_special_test testName)
    run_the_special_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_special_test)

# Function to run an individual test case
function(run_test testName)
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_test)

# Enter one line for each individual test
run_test("getTokenBal_README" "-th")
run_test("getTokenBal_top_three_swarm1" "0xb9e7f8568e08d5659f5d29c4997173d84cdf2607" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98" " 0x6b65353ac052f6c3a0c9688911b078bd025d6ab3" " 0xa25d04fa348f107428708ba19c95d1c79b3744bf" " 4285364" " 3510999" " 2510010" "--byAcct")
run_test("getTokenBal_top_three_swarm2" "0xb9e7f8568e08d5659f5d29c4997173d84cdf2607" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98" " 0x6b65353ac052f6c3a0c9688911b078bd025d6ab3" " 0xa25d04fa348f107428708ba19c95d1c79b3744bf" " 4285364" " 3510999" " 2510010")
run_test("getTokenBal_top_three_swarm3" "0xb9e7f8568e08d5659f5d29c4997173d84cdf2607" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98" " 0x6b65353ac052f6c3a0c9688911b078bd025d6ab3" " 0xa25d04fa348f107428708ba19c95d1c79b3744bf" " 4285364" " 3510999" " 2510010" " --ether")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
