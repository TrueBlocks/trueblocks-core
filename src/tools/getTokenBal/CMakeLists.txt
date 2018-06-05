# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (getTokenBal)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "getTokenBal")
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

# Function to run an special or slow test case
function(run_special_test testName)
    run_the_special_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_special_test)

# Function to run an individual test case
function(run_test testName)
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_test)

# Enter one line for each individual test
# In most of the tests we need to fix the block to compare with static and inmutable results
run_test("getTokenBal_no_params")
run_test("getTokenBal_invalid_params_1" "a" " b" " c")
run_test("getTokenBal_invalid_params_2" "0xd26114cd6EE289AccF82350c8d8487fedB8A0C07" " 21321321312321")
run_test("getTokenBal_invalid_params_3" "21321321312321" " 0xd26114cd6EE289AccF82350c8d8487fedB8A0C07" )
run_test("getTokenBal_invalid_token" "0xd26114cd6EE289AccF82350c8d8487fedB8A0C" " 0x5e44c3e467a49c9ca0296a9f130fc433041aaa28") # token length is not correct
run_test("getTokenBal_invalid_addr" "0xd26114cd6EE289AccF82350c8d8487fedB8A0C07" " 0x5e44c3e467a49c9ca0296a9f130fc433041aaa") # account address length is not correct
run_test("getTokenBal_token_does_not_exist" "0xd26114cd6EE289AccF82350c8d84870000000000" " 0x5e44c3e467a49c9ca0296a9f130fc433041aaa28" " 4000000")
run_test("getTokenBal_acccount_does_not_exist" "0xd26114cd6EE289AccF82350c8d8487fedB8A0C07" " 0x5e44c3e467a49c9ca0296a9f130fc43304000000" " 4000000")
run_test("getTokenBal_single_account" "0xd26114cd6EE289AccF82350c8d8487fedB8A0C07" " 0x5e44c3e467a49c9ca0296a9f130fc433041aaa28" " 4000000")
run_test("getTokenBal_multiple_token_accounts" " 0xd26114cd6EE289AccF82350c8d8487fedB8A0C07" " 0x5e44c3e467a49c9ca0296a9f130fc433041aaa28" " 0xd26114cd6ee289accf82350c8d8487fedb8a0c07" " 4000000")
run_test("getTokenBal_multiple_single_block" "0xd26114cd6EE289AccF82350c8d8487fedB8A0C07" " 0x5e44c3e467a49c9ca0296a9f130fc433041aaa28" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98"  " 4150000") 
run_test("getTokenBal_multiple_multiblock" "0xd26114cd6EE289AccF82350c8d8487fedB8A0C07" " 0x5e44c3e467a49c9ca0296a9f130fc433041aaa28" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98" " 4141000" " 4151000" ) 
run_test("getTokenBal_block_out_of_range" "0xd26114cd6EE289AccF82350c8d8487fedB8A0C07" " 0x5e44c3e467a49c9ca0296a9f130fc433041aaa28" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98" " 6000000" ) 
run_test("getTokenBal_by_account" "0xb9e7f8568e08d5659f5d29c4997173d84cdf2607 " "0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98"  " 0x6b65353ac052f6c3a0c9688911b078bd025d6ab3" " 0xa25d04fa348f107428708ba19c95d1c79b3744bf" " -b" " 4000000") 
run_test("getTokenBal_by_account_long" "0xb9e7f8568e08d5659f5d29c4997173d84cdf2607" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98"  " 0x6b65353ac052f6c3a0c9688911b078bd025d6ab3" " 0xa25d04fa348f107428708ba19c95d1c79b3744bf" " --byAcct" " 4000000")
run_test("getTokenBal_by_account_long_nozero" "0xb9e7f8568e08d5659f5d29c4997173d84cdf2607" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98"  " 0x6b65353ac052f6c3a0c9688911b078bd025d6ab3" " 0xa25d04fa348f107428708ba19c95d1c79b3744bf" " --byAcct" " -n" " 4000000")
run_test("getTokenBal_by_account_long_nozero_long" "0xb9e7f8568e08d5659f5d29c4997173d84cdf2607" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98"  " 0x6b65353ac052f6c3a0c9688911b078bd025d6ab3" " 0xa25d04fa348f107428708ba19c95d1c79b3744bf" " --byAcct" " --noZero" " 4000000")
run_test("getTokenBal_by_account_table" "0xb9e7f8568e08d5659f5d29c4997173d84cdf2607" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98"  " 0x6b65353ac052f6c3a0c9688911b078bd025d6ab3" " 0xa25d04fa348f107428708ba19c95d1c79b3744bf" " --byAcct" " -d" " 4000000" " --total")
run_test("getTokenBal_by_account_table_long" "0xb9e7f8568e08d5659f5d29c4997173d84cdf2607" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98"  " 0x6b65353ac052f6c3a0c9688911b078bd025d6ab3" " 0xa25d04fa348f107428708ba19c95d1c79b3744bf" " --byAcct" " --data" " 4000000")
run_test("getTokenBal_by_account_table_verbose" "0xb9e7f8568e08d5659f5d29c4997173d84cdf2607" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98"  " 0x6b65353ac052f6c3a0c9688911b078bd025d6ab3" " 0xa25d04fa348f107428708ba19c95d1c79b3744bf" " --byAcct" " -d" " -v" " 4000000")
run_test("getTokenBal_by_account_table_verbose_long" "0xb9e7f8568e08d5659f5d29c4997173d84cdf2607" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98"  " 0x6b65353ac052f6c3a0c9688911b078bd025d6ab3" " 0xa25d04fa348f107428708ba19c95d1c79b3744bf" " --byAcct" " -d" " --verbose" " 4000000")
run_test("getTokenBal_help" "-h")
run_test("getTokenBal_help_long" "--help")
run_test("getTokenBal_help_verbose" "--help" " -v")

run_test("getTokenBal_README" "-th")
run_test("getTokenBal_top_three_swarm1" "0xb9e7f8568e08d5659f5d29c4997173d84cdf2607" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98" " 0x6b65353ac052f6c3a0c9688911b078bd025d6ab3" " 0xa25d04fa348f107428708ba19c95d1c79b3744bf" " 4285364" " 3510999" " 2510010" "--byAcct")
run_test("getTokenBal_top_three_swarm2" "0xb9e7f8568e08d5659f5d29c4997173d84cdf2607" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98" " 0x6b65353ac052f6c3a0c9688911b078bd025d6ab3" " 0xa25d04fa348f107428708ba19c95d1c79b3744bf" " 4285364" " 3510999" " 2510010")
run_test("getTokenBal_top_three_swarm3" "0xb9e7f8568e08d5659f5d29c4997173d84cdf2607" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98" " 0x6b65353ac052f6c3a0c9688911b078bd025d6ab3" " 0xa25d04fa348f107428708ba19c95d1c79b3744bf" " 4285364" " 3510999" " 2510010" " --ether")
run_test("getTokenBal_wei" "0xd26114cd6EE289AccF82350c8d8487fedB8A0C07" " 0x5e44c3e467a49c9ca0296a9f130fc433041aaa28" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98"  " 4150000"  " --wei")
run_test("getTokenBal_special_1" "0xd26114cd6EE289AccF82350c8d8487fedB8A0C07" " 0x5e44c3e467a49c9ca0296a9f130fc433041aaa28" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98"  " first")
run_test("getTokenBal_special_2" "0xd26114cd6EE289AccF82350c8d8487fedB8A0C07" " 0x5e44c3e467a49c9ca0296a9f130fc433041aaa28" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98"  "  stateclear" " -t")
run_test("getTokenBal_special_3" "0xd26114cd6EE289AccF82350c8d8487fedB8A0C07" " 0x5e44c3e467a49c9ca0296a9f130fc433041aaa28" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98"  "  stateclear" " daofund")
run_test("getTokenBal_special_and_block_combination" "0xd26114cd6EE289AccF82350c8d8487fedB8A0C07" " 0x5e44c3e467a49c9ca0296a9f130fc433041aaa28" " 0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98"  "  stateclear" " daofund" " 4225341")

run_test("getTokenBal_blockByHash" "0xd26114cd6ee289accf82350c8d8487fedb8a0c07" " 0xcfb98637bcae43c13323eaa1731ced2b716962fd" " 0x45205b0bdb6c9cc9bf0eb3753ae98175c563389b41b2d4a2178593a3415772c1" " 0x966c946dff292e4e5d91169f3007c3c28cd50ca614b18db601806c486f9e6e87")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
