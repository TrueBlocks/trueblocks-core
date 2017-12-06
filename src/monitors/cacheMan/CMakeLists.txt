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
set(PROJ_NAME "monitors")
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
run_test("cacheMan_README"          "-th")
run_test("cacheMan_no_params"              )
run_test("cacheMan_invalid_param_1" "--junk" )
run_test("cacheMan_invalid_param_2" "-l" " --junk" )
run_test("cacheMan_noFile"          "--stats")

# depends on fullblockIndex
run_special_test("cacheMan_list"     "-l"  " Duplicate.cache")
run_special_test("cacheMan_check"    "-c"  " Duplicate.cache")
run_special_test("cacheMan_listTip"  "-l"   " TipJar.cache")
run_special_test("cacheMan_checkTip" "-c"  " TipJar.cache")
run_special_test("cacheMan_fixTip"   "-f"  " TipJar.cache")
run_special_test("cacheMan_merge"    "-md" " TipJar.cache" " Duplicate.cache")
run_special_test("cacheMan_extract"  "-e" " 0" " TipJar.cache" " Duplicate.cache" " --list" " --data")
run_special_test("cacheMan_stats"    "--stats"  " TipJar.cache")

# CParams("-stats",          "report statistics on the cache"),
# CParams("-truncate:<num>", "truncate the cache at block :n (keeps block 'n' and before, implies --fix)"),
# CParams("@s(k)ip",         "skip value for testing"),

