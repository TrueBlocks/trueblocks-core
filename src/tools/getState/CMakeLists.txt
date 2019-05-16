# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (getState)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "getState")
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

## To run only specific tests we define a test configuration named "test_all"
#add_custom_target(test-all COMMAND ${CMAKE_CTEST_COMMAND} -C test_all)

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
run_test("getState_README"        "-th")
run_test("getState_noparams"           )
run_test("getState_bad_address"   "$doa" " 0x12")
run_test("getState_bad_block"     "12" " -1")
run_test("getState_no_address"    "12")
run_test("getState_invalid_address_1"    "0x12")
run_test("getState_invalid_address_2"    "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d3592") # longer than expected, 41 bytes
run_test("getState_invalid_block_1"    "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" "fdfsdf")
run_state_test("getState_dao_1500000"   "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " 1500000")
run_state_test("getState_two_accts"     "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " 0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 1500000" " --fmt" " api")
run_state_test("getState_tip_4"         "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 1500000" " 2000000" " 2500000" " 3000000")
run_state_test("getState_from_file"     "--file:cmd_file")
run_state_test("getState_multiple_accounts" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 0x6f46cf5569aefa1acc1009290c8e043747172d89"  " 4000000")
run_state_test("getState_multiple_accounts_and_blocks_1" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 0x6f46cf5569aefa1acc1009290c8e043747172d89"  " 3500000" " 4000000")
run_state_test("getState_multiple_accounts_and_blocks_2" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 0x6f46cf5569aefa1acc1009290c8e043747172d89" " 0xf27daff52c38b2c373ad2b9392652ddf433303c4" " 3500000" " 3700000" " 4000000")
run_state_test("getState_display_tab_format" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 4000000" " -d" " --json")
run_state_test("getState_display_tab_format_long" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 4000000" " --data")
run_test("getState_no_zero_1" "-n" " 0xf27daff52c38b2c373ad2b9392652ddf433303c5" " 3500000")
run_state_test("getState_no_zero_2" "-n" " 0xf27daff52c38b2c373ad2b9392652ddf433303c5" " 0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 3500000")
run_state_test("getState_no_zero_3" "-n" " 0xf27daff52c38b2c373ad2b9392652ddf433303c4" " 0" " 4000000")
run_state_test("getState_no_zero_4" "-n" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 0x6f46cf5569aefa1acc1009290c8e043747172d89" " 0xf27daff52c38b2c373ad2b9392652ddf433303c4" " 3500000" " 3700000" " 4000000")
run_test("getState_no_zero_long_1" "--nozero" " 0xf27daff52c38b2c373ad2b9392652ddf433303c5" " 3500000" " --api_mode")
run_state_test("getState_no_zero_long_2" "--nozero" " 0xf27daff52c38b2c373ad2b9392652ddf433303c5" " 0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 3500000")
run_state_test("getState_no_zero_long_3" "--nozero" " 0xf27daff52c38b2c373ad2b9392652ddf433303c4" " 0" " 4000000")
run_state_test("getState_no_zero_long_4" "--nozero" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 0x6f46cf5569aefa1acc1009290c8e043747172d89" " 0xf27daff52c38b2c373ad2b9392652ddf433303c4" " 3500000" " 3700000" " 4000000")
run_state_test("getState_verbose" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 4000000" " -v")
run_state_test("getState_verbose_long" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 4000000" " --verbose")
run_test("getState_help" "-h")
run_test("getState_help_long" "--help")
run_test("getState_help_verbose" "--help" " -v")
run_test("getState_special_block_1" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " iceage")
run_state_test("getState_special_block_2" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " stateclear")
run_test("getState_special_block_3" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " first")
run_state_test("getState_wei"             "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 4000000" " --wei")
run_state_test("getState_ether"           "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 4000000" " --ether")
run_state_test("getState_changes"         "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " 1430957-1430966" " --changes")
run_state_test("getState_e_dao_1500000"   "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " 1500000" " --ether")
run_state_test("getState_e_two_accts"     "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " 0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 0x7cb57b5a97eabe94205c07890be4c1ad31e486a8" " 1500000" " --ether")
run_state_test("getState_e_tip_4"         "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 1500000" " 2000000" " 2500000" " 3000000" " --ether")

# Don't run this test since it defaults to latest block which chages at every 14 seconds
#run_test("getState_no_block"    "0xbb9bc244d798123fde783fcc1c72d3bb8c189413")

run_state_test("getState_fmt_txt"      "0xf6f1cd99578ea87b67ae2bf7e9ca3e7e99d0fb98" " --fmt" " txt" " 2000000" " --ether")
run_state_test("getState_fmt_csv"      "0xf6f1cd99578ea87b67ae2bf7e9ca3e7e99d0fb98" " --fmt" " csv" " 2000000")
run_state_test("getState_fmt_json"     "0xf6f1cd99578ea87b67ae2bf7e9ca3e7e99d0fb98" " --fmt" " json" " 2000000" " --dollars")
run_state_test("getState_fmt_api"      "0xf6f1cd99578ea87b67ae2bf7e9ca3e7e99d0fb98" " --fmt" " api" " 2000000")
run_state_test("getState_fmt_junk"     "0xf6f1cd99578ea87b67ae2bf7e9ca3e7e99d0fb98" " --fmt" " junk" " 2000000")
run_state_test("getState_fmt_api_mode" "0xf6f1cd99578ea87b67ae2bf7e9ca3e7e99d0fb98" " --api_mode" " 2000000" " --ether")

run_state_test("getState_bal"        "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " --mode" " bal" " 2500000")
run_state_test("getState_nonce"      "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " --mode" " nonce" " 2500000")
run_state_test("getState_code"       "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " --mode" " code" " 2500000")
run_state_test("getState_storage"    "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " --mode" " storage" " 2500000")
run_state_test("getState_deployed"   "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " --mode" " deployed" " 2500000")
run_state_test("getState_accttype"   "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " --mode" " accttype" " 2500000")
run_state_test("getState_some"       "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " --mode" " some" " 2500000")
run_state_test("getState_all"        "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " --mode" " all" " 2500000")
run_state_test("getState_none"       "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " --mode" " none" " 2500000")
run_state_test("getState_nonce_only" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " --mode" " none" " --mode" " nonce" " 2500000")
run_state_test("getState_mode_fail"  "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " --mode" " junk")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
