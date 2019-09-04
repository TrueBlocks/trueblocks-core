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

# Function to run an individual test case
function(run_test testName)
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_test)

## To run only specific tests we define a test configuration named "test_state"
add_custom_target(test-state COMMAND ${CMAKE_CTEST_COMMAND} -C test_state)

# Function to run state test cases
function(run_state_test testName)
    run_the_state_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_state_test)

# Function to run state test cases
function(api_state_test testName)
    run_the_api_state_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(api_state_test)

# Function to run an api mode test case
function(api_test testName)
    run_the_api_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(api_test)

# Some token and regular accounts
set (OMG "0xd26114cd6ee289accf82350c8d8487fedb8a0c07")
set (SWT "0xb9e7f8568e08d5659f5d29c4997173d84cdf2607")
set (TIP "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359")
set (DAO "0xbb9bc244d798123fde783fcc1c72d3bb8c189413")
set (NET "0xcfb98637bcae43c13323eaa1731ced2b716962fd")
set (EA1 "0x5e44c3e467a49c9ca0296a9f130fc433041aaa28")
set (EA2 "0x6b65353ac052f6c3a0c9688911b078bd025d6ab3")
set (EA3 "0xa25d04fa348f107428708ba19c95d1c79b3744bf")
set (EA4 "0xf6f1cd99578ea87b67ae2bf7e9ca3e7e99d0fb98")
set (EA5 "0x6f46cf5569aefa1acc1009290c8e043747172d89")
set (EA6 "0xf27daff52c38b2c373ad2b9392652ddf433303c5")
set (EA7 "0xf27daff52c38b2c373ad2b9392652ddf433303c4")
set (NON "0xd26114cd6EE289AccF82350c8d84870000000000")
set (NN1 "0x5e44c3e467a49c9ca0296a9f130fc43304000000")
set (BITREX "0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98")

# Enter one line for each individual test
run_test("getState_README"                             "-th")
run_test("getState_noparams"                                )
run_test("getState_help"                                "-h")
run_test("getState_help_long"                       "--help")
run_test("getState_help_verbose"              "--help" " -v")

run_test("getState_invalid_address_1"                    "0x12")
run_test("getState_invalid_address_2"                    "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d3592") # longer than expected, 41 bytes
run_test("getState_invalid_block_1"                      "${TIP}" "fdfsdf")

run_test("getState_bad_address"                         "$doa" " 0x12")
run_test("getState_bad_block"                           "12" " -1")
run_test("getState_no_address"                          "12")
run_state_test("getState_dao_1500000"                    "${DAO}" " 1500000")
run_state_test("getState_two_accts"                      "${DAO}" " ${TIP}" " 1500000" " --fmt" " api")
run_state_test("getState_tip_4"                          "${TIP}" " 1500000" " 2000000" " 2500000" " 3000000")
run_state_test("getState_from_file"                     "--file:cmd_file")
run_state_test("getState_multiple_accounts"              "${TIP}" " ${EA5}"  " 4000000")
run_state_test("getState_multiple_accounts_and_blocks_1" "${TIP}" " ${EA5}"  " 3500000" " 4000000")
run_state_test("getState_multiple_accounts_and_blocks_2" "${TIP}" " ${EA5}" " ${EA7}"              " 3500000" " 3700000" " 4000000")
run_state_test("getState_display_tab_format"             "${TIP}" " 4000000" " -d" " --json")
run_state_test("getState_display_tab_format_long"        "${TIP}" " 4000000" " --data")
run_test("getState_no_zero_1"                            "-n" " ${EA6}" " 3500000")
run_state_test("getState_no_zero_2"                      "-n" " ${EA6}" " ${TIP}" " 3500000")
run_state_test("getState_no_zero_3"                      "-n" " ${EA7}" " 0" " 4000000")
run_state_test("getState_no_zero_4"                      "-n" "${TIP}" " ${EA5}" " ${EA7}"              " 3500000" " 3700000" " 4000000")
api_test("getState_no_zero_long_1"                       "--nozero" " ${EA6}" " 3500000")
run_state_test("getState_no_zero_long_2"                 "--nozero" " ${EA6}" " ${TIP}" " 3500000")
run_state_test("getState_no_zero_long_3"                 "--nozero" " ${EA7}" " 0" " 4000000")
run_state_test("getState_no_zero_long_4"                  "--nozero" "${TIP}" " ${EA5}" " ${EA7}"               " 3500000" " 3700000" " 4000000")
run_state_test("getState_verbose" "${TIP}"                " 4000000" " -v")
run_state_test("getState_verbose_long"                    "${TIP}" " 4000000" " --verbose")
run_test("getState_special_block_1"                       "${TIP}" " iceage")
run_state_test("getState_special_block_2"                 "${TIP}" " stateclear")
run_test("getState_special_block_3"                       "${TIP}" " first")
run_state_test("getState_wei"                             "${TIP}" " 4000000" " --wei")
run_state_test("getState_ether"                           "${TIP}" " 4000000" " --ether")
run_state_test("getState_changes"                         "${DAO}" " 1430957-1430966" " --changes")
run_state_test("getState_e_dao_1500000"                    "${DAO}" " 1500000" " --ether")
run_state_test("getState_e_two_accts"                     "${DAO}" " ${TIP}" " 0x7cb57b5a97eabe94205c07890be4c1ad31e486a8" " 1500000" " --ether")
run_state_test("getState_e_tip_4"                         "${TIP}" " 1500000" " 2000000" " 2500000" " 3000000" " --ether")

run_state_test("getState_fmt_txt"      "${EA4}" " --fmt" " txt" " 2000000" " --ether")
run_state_test("getState_fmt_csv"      "${EA4}" " --fmt" " csv" " 2000000")
run_state_test("getState_fmt_json"     "${EA4}" " --fmt" " json" " 2000000" " --dollars")
run_state_test("getState_fmt_api"      "${EA4}" " --fmt" " api" " 2000000")
run_state_test("getState_fmt_junk"     "${EA4}" " --fmt" " junk" " 2000000")
api_state_test("getState_fmt_api_mode" "${EA4}" " 2000000" " --ether")

run_state_test("getState_bal"          "${TIP}" " --mode" " bal" " 2500000")
run_state_test("getState_nonce"        "${TIP}" " --mode" " nonce" " 2500000")
run_state_test("getState_code"         "${TIP}" " --mode" " code" " 2500000")
run_state_test("getState_storage"      "${TIP}" " --mode" " storage" " 2500000")
run_state_test("getState_deployed"     "${TIP}" " --mode" " deployed" " 2500000")
run_state_test("getState_accttype"     "${TIP}" " --mode" " accttype" " 2500000")
run_state_test("getState_some"         "${TIP}" " --mode" " some" " 2500000")
run_state_test("getState_all"          "${TIP}" " --mode" " all" " 2500000")
run_state_test("getState_none"         "${TIP}" " --mode" " none" " 2500000")
run_state_test("getState_nonce_only"   "${TIP}" " --mode" " none" " --mode" " nonce" " 2500000")
run_state_test("getState_mode_fail"    "${TIP}" " --mode" " junk" " 2500000")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
