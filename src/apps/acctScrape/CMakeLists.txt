# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (acctScrape)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "acctScrape")
set(PROJ_NAME "apps")
add_executable(${TOOL_NAME} ${SOURCE_FILES})

# Add the project static libs at linking
target_link_libraries (${TOOL_NAME} ${BASE_LIBS})

# Testing
# Define paths to test folder and gold folder
set(TEST_EXE "${EXECUTABLE_OUTPUT_PATH}/${TOOL_NAME}")
set(TEST_PATH "${TEST_PATH}/${PROJ_NAME}/${TOOL_NAME}")
set(GOLD_PATH "${GOLD_PATH}/${PROJ_NAME}/${TOOL_NAME}")

# Additional target to make the README.md
build_readme(${CMAKE_CURRENT_SOURCE_DIR} ${TOOL_NAME})

# To run only specific tests we define a test configuration named "test_all"
add_custom_target(test-all COMMAND ${CMAKE_CTEST_COMMAND} -C test_all)

# Function to run an special or slow test case
function(run_special_test testName)
    run_the_special_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_special_test)

# Function to run an individual test case
function(run_test testName)
    run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_test)

# Enter one line for each individual test
#run_the_test(${TEST_PATH} ${GOLD_PATH} "acctScrape_preload" ${EXECUTABLE_OUTPUT_PATH}/bloomMan " --rewrite" " 46147")
run_test("acctScrape_no_params")
run_test("acctScrape_help" "-th")
run_test("acctScrape_fail_list1" "--list")
run_test("acctScrape_fail_list2" "0x1234567890123456789012345678901234567890")
run_test("acctScrape_list_test" "--list" " 0x1234567890123456789012345678901234567890")

# Installation steps, this tool is renamed at installation
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
