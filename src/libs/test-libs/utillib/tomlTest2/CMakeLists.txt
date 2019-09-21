# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (tomlTest2)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin/test")

# Define the executable to be generated
set(TOOL_NAME "tomlTest2")
set(PROJ_NAME "libs/utillib")
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
#
run_test("tomlTest2_acctExport"  "acctExport.toml")
run_test("tomlTest2_acctScrape"  "acctScrape.toml")
run_test("tomlTest2_blockScrape" "blockScrape.toml")
run_test("tomlTest2_cacheMan"    "cacheMan.toml")
run_test("tomlTest2_chifra"      "chifra.toml")
run_test("tomlTest2_ethslurp"    "ethslurp.toml")
run_test("tomlTest2_getAccounts" "getAccounts.toml")
run_test("tomlTest2_getBlock"    "getBlock.toml")
run_test("tomlTest2_getState"    "getState.toml")
run_test("tomlTest2_getTrans"    "getTrans.toml")
run_test("tomlTest2_grabABI"     "grabABI.toml")
run_test("tomlTest2_quickBlocks" "quickBlocks.toml")
run_test("tomlTest2_whenBlock"   "whenBlock.toml")
