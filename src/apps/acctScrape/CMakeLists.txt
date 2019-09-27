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

# Target to run only specific (long-running) test cases
add_custom_target(test-all COMMAND ${CMAKE_CTEST_COMMAND} -C test_all)
add_custom_target(test-state COMMAND ${CMAKE_CTEST_COMMAND} -C test_state)

# Functions to run test cases
function(run_test testName)
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_test)
function(run_special_test testName)
    run_the_special_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_special_test)
function(run_state_test testName)
    run_the_state_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_state_test)
function(api_test testName)
    run_the_api_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(api_test)
function(api_special_test testName)
    run_the_api_special_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(api_special_test)
function(api_state_test testName)
    run_the_api_state_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(api_state_test)

# prepare the data
run_the_api_test(${TEST_PATH} ${GOLD_PATH} "acctScrape_chifra_rm" ${EXECUTABLE_OUTPUT_PATH}/chifra " rm" " --yes" " 0x001d14804b399c6ef80e64576f657660804fec0b")
run_the_test(${TEST_PATH} ${GOLD_PATH} "acctScrape_preload_list" ${EXECUTABLE_OUTPUT_PATH}/acctScrape " 0x001d14804b399c6ef80e64576f657660804fec0b")

# Enter one line for each individual test
run_test("acctScrape_no_params")
run_test("acctScrape_help" "-th")
run_test("acctScrape_fail_list1" "--list")
run_test("acctScrape_fail_list2" "0x1234567890123456789012345678901234567890")
run_test("acctScrape_list_test" " 0x001d14804b399c6ef80e64576f657660804fec0b")

# Installation steps, this tool is renamed at installation
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
