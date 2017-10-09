# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (getTrans)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "getTrans")
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

# Weird, but we have to call the getBlock code to make sure the block is in the cache
run_the_test(${TEST_PATH} ${GOLD_PATH} "getTrans_preload" ${EXECUTABLE_OUTPUT_PATH}/getBlock " 3657480" " -o")

# Enter one line for each individual test
run_test("getTrans_README"            "-th")
run_test("getTrans_no_input"          )
run_test("getTrans_bad_hash"          "0x0012")
run_test("getTrans_invalid_option_1"  "-p")
run_test("getTrans_invalid_option_2"  "--other")
run_test("getTrans_invalid_param_1"   ".th")
run_test("getTrans_invalid_param_2"   ".whatever")
run_test("getTrans_invalid_param_3"   "1233123")
run_test("getTrans_by_txhash"          "0x6afbe0f0ea3613edd6b84b71260836c03bddce81604f05c81a070cd671d3d765")
run_test("getTrans_by_bnun_and_txid"   "4000000.5")
run_test("getTrans_by_bhash_and_txid"  "0xb8a3f7f5cfc1748f91a684f20fe89031202cbadcd15078c49b85ec2a57f43853.4" " -v" " --parity")
run_test("getTrans_from_node"          "-r" " 4000000.5")
run_test("getTrans_from_node_long"     "--raw" " 4000000.5")
run_test("getTrans_help"               "-h")
run_test("getTrans_help_long"          "--help")

run_test("getTrans_verbose_blk_idx1"   "-v" " 0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.3")
run_test("getTrans_verbose_blk_idx2"   "--verbose" " 0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.3")
run_test("getTrans_verbose_blk_idx3"   "0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.3" " -v")

run_test("getTrans_by_blk_hash_good"   "0x693b52d0fc09ec86d86aa117dd2b397116b6031cf2b672f8673c4441b9dc190f.55" " --verbose")
# this test case should fail, but it doesn't. It calls for a transaction (#155) that does not exist since this block only has 94 transactions
#run_test("getTrans_by_blk_hash_bad"    "0x693b52d0fc09ec86d86aa117dd2b397116b6031cf2b672f8673c4441b9dc190f.155" " -v")

run_test("getTrans_three_ways"         "0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.79" " -v" " 3657480.79" " 0xda2f27fffb36c90c74fc53bd8b721074e87203c727640c95c4c802296e3bf8ad")
# this test case should fail but does not because if one has a --file: option, the rest of the options (other than the built-ins) are ignored
#run_test("getTrans_by_file"            "--bad --file:cmd_file")
run_test("getTrans_by_file"            "--file:cmd_file")
run_test("getTrans_by_file_long_v"     "--file:cmd_file" " --verbose" " --parity")
run_test("getTrans_by_file_v"          "--verbose"       " --file:cmd_file")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
