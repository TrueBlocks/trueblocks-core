# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (whenBlock)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "whenBlock")
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

# Function to run an special or slow test case
function(api_special_test testName)
    run_the_api_special_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(api_special_test)

# Function to run an individual test case
function(run_test testName)
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_test)

# Function to run an api mode test case
function(api_test testName)
    run_the_api_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(api_test)

# Enter one line for each individual test
run_test("whenBlock_README"                               "-th")
run_test("whenBlock_noparams"                             )
run_test("whenBlock_help"                                 "-h")
run_test("whenBlock_long_help"                            "--help")
run_test("whenBlock_special_block_iceage"                 "iceage")
run_test("whenBlock_special_block_daohack"                "daohack" " -v")
run_test("whenBlock_special_block_daofork"                "daofork" " -v")
run_test("whenBlock_special_block_spurious"               "spurious")
run_test("whenBlock_special_block_not_found"              "idonotexist")
run_test("whenBlock_customized_1"                         "myblock")  # customized
run_test("whenBlock_upgrade_test"                         "--help")  # customized
run_test("whenBlock_blockByHash"                          "0xd19bc98080cd70951fad3951cef2d512e6334aa3c7e64a38985c955d4d2e273b")

run_test("whenBlock_fmt_txt"                              "1001001" " --fmt" " txt")
run_test("whenBlock_fmt_csv"                              "1001001" " --fmt" " csv")
run_test("whenBlock_fmt_json"                             "1001001" " --fmt" " json")
run_test("whenBlock_fmt_api"                              "1001001" " --fmt" " api")
api_test("whenBlock_fmt_defaut"                           "1001001")
api_test("whenBlock_fmt_api_mode"                         "1001001")
api_test("whenBlock_fmt_api_mode_fmt"                     "1001001" " --fmt" " txt")
run_test("whenBlock_fmt_junk"                             "1001001" " --fmt" " junk")

run_special_test("whenBlock_02"                           "--file:blocks.txt" " -v")
run_special_test("whenBlock_03"                           "--file:dates.txt")
run_special_test("whenBlock_invalid_option_1"             "-x")
run_special_test("whenBlock_invalid_option_2"             "a")
run_special_test("whenBlock_invalid_option_3"             "0-0")
run_special_test("whenBlock_invalid_long_option_1"        "--none")
run_special_test("whenBlock_invalid_long_option_2"        " 0" " --invalid")
run_special_test("whenBlock_invalid_block_1"              "0x0a")
run_special_test("whenBlock_invalid_block_2"              "1ppppppp")
run_special_test("whenBlock_out_of_range_block"           "40000000000000000000000000000")
run_special_test("whenBlock_mixed_options"                "-vlh")
run_special_test("whenBlock_mixed_block_and_date"         "2017-03-02" " 123123")
run_special_test("whenBlock_first_block"                  "0")
run_special_test("whenBlock_intermediate_block_1"         "1000000")
run_special_test("whenBlock_intermediate_block_2"         "2677160")
run_special_test("whenBlock_not_present_block"            "8000000")
run_special_test("whenBlock_block_with_leading_zeroes"    "000000011")
run_special_test("whenBlock_invalid_alone_syntax_2"       "2015-03-02")
run_special_test("whenBlock_invalid_alone_syntax_3"       "pppppp1")
run_special_test("whenBlock_invalid_alone_syntax_4"       "0pppppp")
run_special_test("whenBlock_invalid_long_alone_syntax_2"  "2015-03-02")
run_special_test("whenBlock_invalid_long_alone_syntax_3"  "pppppp1")
run_special_test("whenBlock_invalid_long_alone_syntax_4"  "0pppppp")
run_special_test("whenBlock_invalid_list_syntax_1"        "-l")
run_special_test("whenBlock_invalid_list_syntax_2"        "-l" " 213123213")
run_special_test("whenBlock_invalid_list_syntax_3"        "-l" " 2015-03-02")
run_special_test("whenBlock_invalid_list_syntax_4"        "-l" " 23123")
run_special_test("whenBlock_invalid_long_list_syntax_1"   "--list")
run_special_test("whenBlock_invalid_long_list_syntax_2"   "--list"   " 213123213")
run_special_test("whenBlock_invalid_long_list_syntax_3"   "--list"   " -k" " 2015-03-02")
run_special_test("whenBlock_invalid_long_list_syntax_4"   "--list"   " 2015-03-02")
run_special_test("whenBlock_list_dates"                   "-l")
run_special_test("whenBlock_list_dates_long"              "--list")
run_special_test("whenBlock_invalid_verbose_syntax"       "-v")
run_special_test("whenBlock_verbose_valid_block"          "-v"        " 1000")
run_special_test("whenBlock_verbose_level_1"              "-v:1"      " 1000")
run_special_test("whenBlock_verbose_level_2"              "-v:j"      " 1000")
run_special_test("whenBlock_invalid_long_verbose_syntax"  "--verbose")
run_special_test("whenBlock_long_verbose_valid_block"     "--verbose" " 1000")
run_special_test("whenBlock_combined_options_1"           "-v" " 10000")
run_special_test("whenBlock_combined_options_2"           "--verbose" " 10000")
run_special_test("whenBlock_combined_options_3"           "--verbose" " 10000")
run_special_test("whenBlock_special_block_first"          "first" " -v")
run_special_test("whenBlock_homestead_customized"         "homestead" " -v" " --fmt" " json")  # customized
run_special_test("whenBlock_special_block_daofund"        "daofund" " -v")
run_special_test("whenBlock_special_block_tangerine"      "tangerine" " -v")
run_special_test("whenBlock_special_block_stateclear"     "stateclear" " -v")
run_special_test("whenBlock_special_block_multi1"         "stateclear" " iceage" " 4001001" " -v")
run_special_test("whenBlock_special_block_multi2"         "stateclear" " iceage" " 4001001" " junk" " -v")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
