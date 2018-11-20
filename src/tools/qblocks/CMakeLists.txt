# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (qblocks)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "qblocks")
set(PROJ_NAME "tools")
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
run_test("qblocks_README"        "-th")
run_test("qblocks_help"          "--help")
run_test("qblocks_invalid"       "--invalid")
run_test("qblocks_help_block"    "block"    " -h")
run_test("qblocks_help_trans"    "trans"    " -h")
run_test("qblocks_help_receipt"  "receipt"  " -h")
run_test("qblocks_help_logs"     "logs"     " -h")
run_test("qblocks_help_trace"    "trace"    " -h")
run_test("qblocks_help_bloom"    "bloom"    " -h")
run_test("qblocks_help_accounts" "accounts" " -h")
run_test("qblocks_help_balance"  "balance"  " -h")
run_test("qblocks_help_tokenbal" "tokenbal" " -h")
run_test("qblocks_help_contract" "contract" " -h")
run_test("qblocks_help_names"    "name"     " -h")
run_test("qblocks_help_when"     "when"     " -h")
run_test("qblocks_help_where"    "where"    " -h")

run_test("qblocks_help_info1"    "-info")
run_test("qblocks_help_info2"    "-i")
run_test("qblocks_help_info3"    "block"    " -i")

run_test("qblocks_data_block"    "block"    " 2001001")
run_test("qblocks_data_trans"    "trans"    " 2001002.0")
run_test("qblocks_data_receipt"  "receipt"  " 2001002.0")
run_test("qblocks_data_logs"     "logs"     " 2001002.0")
run_test("qblocks_data_trace"    "trace"    " 2001002.0")
run_test("qblocks_data_bloom"    "bloom"    " 2001001")
run_test("qblocks_data_accounts" "accounts")
run_test("qblocks_data_balance"  "balance"  " 0xf7b098298f7c69fc14610bf71d5e02c60792894c" " 2001001")
run_test("qblocks_data_tokenbal" "tokenbal" " 0x4775daf2fdc3b3c513ef2a6e07862c27f9d6f781" " 0xaec2e87e0a235266d9c5adc9deb4b2e29b54d009" " 2367305")
run_test("qblocks_data_contract" "contract" " 0x4775daf2fdc3b3c513ef2a6e07862c27f9d6f781")
run_test("qblocks_data_names"    "name"     " sing")
run_test("qblocks_data_when"     "when"     " 2001001")
run_test("qblocks_data_where"    "where"    " 2001001")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
