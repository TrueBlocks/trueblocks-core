# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (cacheStatus)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "cacheStatus")
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

# Function to run an individual test case
function(run_test testName)
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_test)

# To run only specific tests we define a test configuration named "test_all"
add_custom_target(test-all COMMAND ${CMAKE_CTEST_COMMAND} -C test_all)

# Function to run an special or slow test case
function(run_special_test testName)
    run_the_special_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_special_test)

## To run only specific tests we define a test configuration named "test_state"
add_custom_target(test-state COMMAND ${CMAKE_CTEST_COMMAND} -C test_state)

# Function to run state test cases
function(run_state_test testName)
    run_the_state_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_state_test)

# Enter one line for each individual test
run_test("cacheStatus_README"                 "-th")
run_test("cacheStatus_no_params"              )
run_test("cacheStatus_invalid_param_1"        "--junk" )
run_test("cacheStatus_invalid_param_2"        "-l" " --junk" )
run_test("cacheStatus_noFile"                 "--stats")

# depends on fullblockIndex
run_special_test("cacheStatus_list"           "-l"  " 0x9876543210987654321098765432109876543210")
run_special_test("cacheStatus_check"          "-c"  " 0x9876543210987654321098765432109876543210")
run_special_test("cacheStatus_listTip"        "-l"  " 0x5555533333555553333355555333335555533333")
run_special_test("cacheStatus_checkTip"       "-c"  " 0x5555533333555553333355555333335555533333")
run_special_test("cacheStatus_fixTip"         "--fix" " 0x5555533333555553333355555333335555533333")
run_special_test("cacheStatus_txtTip"         "--fmt"  " txt" " 0x5555533333555553333355555333335555533333")
run_special_test("cacheStatus_csvTip"         "--fmt"  " csv" " 0x5555533333555553333355555333335555533333")
run_special_test("cacheStatus_jsonTip"        "--fmt"  " json" " 0x5555533333555553333355555333335555533333")

#run_special_test("cacheStatus_merge"          "-md" " 0x5555533333555553333355555333335555533333" " 0x9876543210987654321098765432109876543210")
#run_special_test("cacheStatus_merge_list"     "-l"  " merged")
#run_special_test("cacheStatus_merge_remove"   "-r"  " merged")
#run_special_test("cacheStatus_merge_rlist"    "-l"  " merged")
#run_special_test("cacheStatus_merge_import"   "-i"  " merged")
#run_special_test("cacheStatus_merge_ilist"    "-l"  " merged")

#run_special_test("cacheStatus_extract"        "-e"  " 0" " 0x5555533333555553333355555333335555533333" " 0x9876543210987654321098765432109876543210" " --list" " --data")
#run_special_test("cacheStatus_extract_list"   "-l"  " extract" " --data")

# COption("-truncate:<num>", "truncate the cache at block :n (keeps block 'n' and before, implies --fix)"),
# COption("@s(k)ip",         "skip value for testing"),

# Installation steps, this tool is renamed at installation
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
