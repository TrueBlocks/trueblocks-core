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
set(README_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
add_custom_target(readme COMMAND ${TEST_EXE} "-th" ">" "${README_FILE}" "2>&1")

# Function to run an individual test case
function(run_test testName)
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGV1})
endfunction(run_test)

# Enter one line for each individual test
run_test("getBalance_README"      "-th")
run_test("getBalance_noparams"         )
run_test("getBalance_bad_address" "$doa 0x12")
run_test("getBalance_bad_block"   "12 -1")
run_test("getBalance_no_address"  "12")
# We don't run this test because it would change every time
# it's run since without a block, it defaults to latest block
# run_test("getBalance_no_block"    "0xbb9bc244d798123fde783fcc1c72d3bb8c189413")
run_test("getBalance_dao_1500000"   "0xbb9bc244d798123fde783fcc1c72d3bb8c189413 1500000")
run_test("getBalance_two_accts"     "0xbb9bc244d798123fde783fcc1c72d3bb8c189413 $tip 1500000")
run_test("getBalance_tip_4"         "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359 1500000 2000000 2500000 3000000")

# The following tests are identical, but use the --ether command
#broken run_test("getBalance_e_dao_1500000" "0xbb9bc244d798123fde783fcc1c72d3bb8c189413 1500000 -e")
#broken run_test("getBalance_e_two_accts"   "0xbb9bc244d798123fde783fcc1c72d3bb8c189413 $tip 1500000 -e")
#broken run_test("getBalance_e_tip_4"       "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359 1500000 2000000 2500000 3000000 -e")

run_test("getBalance_from_file"     "--file:cmd_file")
