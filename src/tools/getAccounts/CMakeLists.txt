# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (getAccounts)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "getAccounts")
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
run_test("getAccounts_README"              "-th")
run_test("getAccounts_run"                 "" )
run_test("getAccounts_help"                "-h")
run_test("getAccounts_help_long"           "--help")
run_test("getAccounts_invalid_option"      "-x" )
run_test("getAccounts_bad_option"          "-a" )
run_test("getAccounts_verbose"             "-v" )
run_test("getAccounts_verbose_long"        "--verbose" )

run_test("getAccounts_named"               "-n" )
run_test("getAccounts_named_long"          "--named" )
run_test("getAccounts_prefund"             "-p" )
run_test("getAccounts_prefund_long"        "--prefund" )
run_test("getAccounts_owned"               "-o" )
run_test("getAccounts_owned_long"          "--owned" )
run_test("getAccounts_custom"              "-c" )
run_test("getAccounts_custom_long"         "--custom" )

run_test("getAccounts_named_custom"        "--named" " --custom" )
run_test("getAccounts_named_custom_owned"  "--named" " --custom" " --owned")
run_test("getAccounts_all"                 "-ncop")

run_test("getAccounts_all_addrOnly"        "--named" " -c" " --owned" " -p" " -a")

run_test("getAccounts_fmt_csv"  "--named" " --custom" " --owned" " --fmt" " csv")
run_test("getAccounts_fmt_txt"  "--named" " --custom" " --owned" " --fmt" " txt")
run_test("getAccounts_fmt_json"  "--named" " --custom" " --owned" " --fmt" " json")
run_test("getAccounts_fmt_api"  "--named" " --custom" " --owned" " --fmt" " api")
run_test("getAccounts_fmt_api_mode"  "--named" " --custom" " --owned" " --api_mode")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
