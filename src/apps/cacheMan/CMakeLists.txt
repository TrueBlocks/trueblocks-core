# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (cacheMan)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "cacheMan")
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

## To run only specific tests we define a test configuration named "test_state"
add_custom_target(test-state COMMAND ${CMAKE_CTEST_COMMAND} -C test_state)

# Function to run state test cases
function(run_state_test testName)
    run_the_state_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_state_test)

# Function to run an special or slow test case
function(run_special_test testName)
    run_the_special_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_special_test)

# Function to run an individual test case
function(run_test testName)
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_test)

# Enter one line for each individual test
run_test("cacheMan_README"          "-th")
run_test("cacheMan_no_params"              )
run_test("cacheMan_invalid_param_1" "--junk" )
run_test("cacheMan_invalid_param_2" "-l" " --junk" )
run_test("cacheMan_noFile"          "--stats")

# depends on fullblockIndex
run_special_test("cacheMan_list"           "-l"  " Duplicate.acct.bin")
run_special_test("cacheMan_check"          "-c"  " Duplicate.acct.bin")
run_special_test("cacheMan_listTip"        "-l"  " TipJar.acct.bin")
run_special_test("cacheMan_checkTip"       "-c"  " TipJar.acct.bin")
run_special_test("cacheMan_fixTip"         "-f"  " TipJar.acct.bin")
run_special_test("cacheMan_merge"          "-md" " TipJar.acct.bin" " Duplicate.acct.bin")
run_special_test("cacheMan_merge_list"     "-l"  " merged.acct.bin")
run_special_test("cacheMan_merge_remove"   "-r"  " merged.acct.bin")
run_special_test("cacheMan_merge_rlist"    "-l"  " merged.acct.bin")
run_special_test("cacheMan_merge_import"   "-i"  " merged.acct.bin")
run_special_test("cacheMan_merge_ilist"    "-l"  " merged.acct.bin")
run_special_test("cacheMan_extract"        "-e"  " 0" " TipJar.acct.bin" " Duplicate.acct.bin" " --list" " --data")
run_special_test("cacheMan_extract_list"   "-l"  " extract.acct.bin" " --data")

run_state_test("cacheMan_balanceFail" "cache/0x001d14804b399c6ef80e64576f657660804fabcd.acct.bin" " --cacheBals")
run_state_test("cacheMan_balanceGet" "cache/0x001d14804b399c6ef80e64576f657660804fec0b.acct.bin" " --cacheBals")
run_state_test("cacheMan_balances" "cache/0x001d14804b399c6ef80e64576f657660804fec0b.acct.bin" " --balances")

# COption("-truncate:<num>", "truncate the cache at block :n (keeps block 'n' and before, implies --fix)"),
# COption("@s(k)ip",         "skip value for testing"),

# Installation steps, this tool is renamed at installation
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
