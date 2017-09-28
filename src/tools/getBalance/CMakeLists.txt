# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (getBalance)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "getBalance")
set(PROJ_NAME "tools")
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

# Function to run an special or slow test case
function(run_special_test testName)
    run_the_special_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_special_test)

# Function to run an individual test case
function(run_test testName)
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_test)

# Enter one line for each individual test
run_test("getBalance_README"        "-th")
run_test("getBalance_noparams"           )
run_test("getBalance_bad_address"   "$doa" " 0x12")
run_test("getBalance_bad_block"     "12" " -1")
run_test("getBalance_no_address"    "12")
run_test("getBalance_dao_1500000"   "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " 1500000")
run_test("getBalance_two_accts"     "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " $tip" " 1500000")
run_test("getBalance_tip_4"         "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 1500000" " 2000000" " 2500000" " 3000000")
run_test("getBalance_from_file"     "--file:cmd_file")
#run_test("getBalance_multiple_accounts" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 0x6f46cf5569aefa1acc1009290c8e043747172d89"  " 4000000")
#run_test("getBalance_multiple_accounts_and_blocks_1" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 0x6f46cf5569aefa1acc1009290c8e043747172d89"  " 3500000" " 4000000")
#run_test("getBalance_multiple_accounts_and_blocks_2" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 0x6f46cf5569aefa1acc1009290c8e043747172d89" " 0xf27daff52c38b2c373ad2b9392652ddf433303c4" " 3500000" " 3700000" " 4000000")
#run_test("getBalance_display_tab_format" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 4000000" " -d")
#run_test("getBalance_display_tab_format_long" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 4000000" " --data")
#run_test("getBalance_no_zero_1" "-n" " 0xf27daff52c38b2c373ad2b9392652ddf433303c4" " 3500000")
#run_test("getBalance_no_zero_2" "-n" " 0xf27daff52c38b2c373ad2b9392652ddf433303c4" " 3500000" " 4000000")
#run_test("getBalance_no_zero_3" "-n" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 0x6f46cf5569aefa1acc1009290c8e043747172d89" " 0xf27daff52c38b2c373ad2b9392652ddf433303c4" " 3500000" " 3700000" " 4000000")
#run_test("getBalance_no_zero_long_1" "--noZero" " 0xf27daff52c38b2c373ad2b9392652ddf433303c4" " 3500000")
#run_test("getBalance_no_zero_long_2" "--noZero" " 0xf27daff52c38b2c373ad2b9392652ddf433303c4" " 3500000" " 4000000")
#run_test("getBalance_no_zero_long_3" "--noZero" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 0x6f46cf5569aefa1acc1009290c8e043747172d89" " 0xf27daff52c38b2c373ad2b9392652ddf433303c4" " 3500000" " 3700000" " 4000000")
#run_test("getBalance_verbose" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 4000000" " -v")
#run_test("getBalance_verbose_long" "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 4000000" " --verbose")
#run_test("getBalance_help" "-h")
#run_test("getBalance_help_long" "--help")

# Don't run these test since the --ether option is not implemented
#run_test("getBalance_e_dao_1500000" "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " 1500000" " --ether")
#run_test("getBalance_e_two_accts"   "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " $tip" " 1500000" " --ether")
#run_test("getBalance_e_tip_4"       "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " 1500000" " 2000000" " 2500000" " 3000000" " --ether")

# Don't run this test since it defaults to latest block which chages at every 14 seconds
#run_test("getBalance_no_block"    "0xbb9bc244d798123fde783fcc1c72d3bb8c189413")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
