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

# Enter one line for each individual test
run_test("cacheStatus_README"           "-th")
run_test("cacheStatus_no_params"        )
run_test("cacheStatus_invalid_param_1"  "--junk" )
run_test("cacheStatus_invalid_param_2"  "-l" " --junk" )

api_test("cacheStatus_scraper"          "index")
api_test("cacheStatus_index_start"      "index" " --start" " 100")
api_test("cacheStatus_index_start_bad"  "index" " --start" " 100000000")
run_test("cacheStatus_monitors"         "monitors")
run_test("cacheStatus_names"            "names")
run_test("cacheStatus_abis"             "abis")
run_test("cacheStatus_blocks"           "blocks")
run_test("cacheStatus_txs"              "txs")
run_test("cacheStatus_traces"           "traces")
run_test("cacheStatus_slurps"           "slurps")
run_test("cacheStatus_prices"           "prices")
run_test("cacheStatus_some"             "some")
run_state_test("cacheStatus_all"        "all")

run_test("cacheStatus_config_bad"       "config")
run_test("cacheStatus_config_get"       "--config-get")
run_test("cacheStatus_config_set_bad"   "--config-set")
run_test("cacheStatus_config_set"       "--config-set" " string")

run_special_test("cacheStatus_items_monitors"          "monitors" " --details")
run_special_test("cacheStatus_items_abis"              "abis"     " --details")
run_special_test("cacheStatus_items_slurps"            "slurps"   " --details")
run_special_test("cacheStatus_items_prices"            "prices"   " --details")
run_special_test("cacheStatus_items_some"              "some"     " --details")
run_special_test("cacheStatus_items_scraper_monitors"  "index"    " monitors" " --details")

run_special_test("cacheStatus_three"                   "abis"     " slurps" " monitors")
run_special_test("cacheStatus_items_three"             "abis"     " slurps" " monitors" " --details")

# Installation steps, this tool is renamed at installation
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
