# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (ethQuote)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "ethQuote")
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
run_test("ethQuote_README"          "-th")
run_test("ethQuote_no_params"            )
run_test("ethQuote_verbose_00"      "-v" " --fmt" " json")
run_test("ethQuote_at_00"           "-a" " 1438272138")
run_test("ethQuote_period_01"       "--period" " 240" " -v:2" " --fmt" " json")
run_test("ethQuote_when_01"         "--when" " 19")

# Slower tests
run_special_test("ethQuote_invalid_param_1" "-x" )
run_special_test("ethQuote_at_invalid"      "-a")
run_special_test("ethQuote_period_invalid"  "-p")
run_special_test("ethQuote_when_invalid"    "-w")
run_special_test("ethQuote_period_00"       "-p" " 60")
run_special_test("ethQuote_period_02"       "--period" " 12")
run_special_test("ethQuote_when_00"         "-w" " 14")
run_special_test("ethQuote_verbose_01"      "-v:1" " --fmt" " api")
run_special_test("ethQuote_from_file"       "--file:cmd_file")
#run_special_test("ethQuote_bitcoin_eth"     "--pair:BTC_ETH")  # customized
#run_special_test("ethQuote_usd_bitcoin"     "--pair:USDT_BTC")
#run_special_test("ethQuote_usd_litecoin"    "--pair:USDT_LTC")  # customized

run_test("ethQuote_fmt_txt"          "-d" " --fmt" " txt")
run_test("ethQuote_fmt_csv"          "-d" " --fmt" " csv")
run_test("ethQuote_fmt_json"         "-d" " --fmt" " json")
run_test("ethQuote_fmt_api"          "-d" " --fmt" " api")
run_test("ethQuote_fmt_api_mode"     "-d" " --api_mode")
run_test("ethQuote_fmt_api_mode_fmt" "-d" " --api_mode" " --fmt" " txt")
run_test("ethQuote_fmt_defaut"       "-d")
run_test("ethQuote_fmt_junk"         "-d" " --fmt" " junk")

# Installation steps, this tool is renamed at installation
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
