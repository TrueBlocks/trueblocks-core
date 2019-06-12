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

# Weird, but we have to call the getBlock code to make sure the block is in the cache
run_the_test(${TEST_PATH} ${GOLD_PATH} "getTrans_preload" ${EXECUTABLE_OUTPUT_PATH}/getBlock " 3657480" " -o")
# Weird, but we have to get ABIs so the articulation tests work
run_the_test(${TEST_PATH} ${GOLD_PATH} "getTrans_preload_abi1" ${EXECUTABLE_OUTPUT_PATH}/grabABI " 0x1234567812345678123456781234567812345678")
run_the_test(${TEST_PATH} ${GOLD_PATH} "getTrans_preload_abi2" ${EXECUTABLE_OUTPUT_PATH}/grabABI " 0x6090a6e47849629b7245dfa1ca21d94cd15878ef")
run_the_test(${TEST_PATH} ${GOLD_PATH} "getTrans_preload_abi3" ${EXECUTABLE_OUTPUT_PATH}/grabABI " 0xebf2bf80c3ddb0b6ac2f88be8dceec7bf12dff22")
run_the_test(${TEST_PATH} ${GOLD_PATH} "getTrans_preload_abi4" ${EXECUTABLE_OUTPUT_PATH}/grabABI " 0x2bd2326c993dfaef84f696526064ff22eba5b362")
run_the_test(${TEST_PATH} ${GOLD_PATH} "getTrans_preload_abi5" ${EXECUTABLE_OUTPUT_PATH}/grabABI " 0x314159265dd8dbb310642f98f50c066173c1259c")
run_the_test(${TEST_PATH} ${GOLD_PATH} "getTrans_preload_abi6" ${EXECUTABLE_OUTPUT_PATH}/grabABI " 0x314159265dd8dbb310642f98f50c066173c1259b")
run_the_test(${TEST_PATH} ${GOLD_PATH} "getTrans_preload_abi7" ${EXECUTABLE_OUTPUT_PATH}/grabABI " 0x159cf1e9ae58211b588f5e3bf1d7e423952d959b")

run_test("getTrans_README"            "-th")
run_test("getTrans_no_input"          )
run_test("getTrans_bad_hash"          "0x0012")
run_test("getTrans_invalid_option_1"  "-p")
run_test("getTrans_invalid_option_2"  "--other")
run_test("getTrans_invalid_param_1"   ".th")
run_test("getTrans_invalid_param_2"   ".whatever")
run_test("getTrans_invalid_param_3"   "1233123")
run_test("getTrans_by_txhash"          "0x6afbe0f0ea3613edd6b84b71260836c03bddce81604f05c81a070cd671d3d765")
run_test("getTrans_by_bnun_and_txid"   "4000002.5" " --trace")
run_test("getTrans_by_bhash_and_txid"  "0xb8a3f7f5cfc1748f91a684f20fe89031202cbadcd15078c49b85ec2a57f43853.4" " --fmt " " json" " --parity")
run_test("getTrans_keys_combination" "0x6afbe0f0ea3613edd6b84b71260836c03bddce81604f05c81a070cd671d3d765" " 4000000.5" " 0xb8a3f7f5cfc1748f91a684f20fe89031202cbadcd15078c49b85ec2a57f43853.4" " --fmt " " json")
run_test("getTrans_mulit_line_mulit_txs" "--file:cmd_file_1" " --fmt" " json")
run_test("getTrans_from_node"          "--veryRaw" " 4000000.5")
run_test("getTrans_from_node_long"     "--veryRaw" " 4000000.5")
run_test("getTrans_help"               "-h")
run_test("getTrans_help_long"          "--help")
run_test("getTrans_should_be_error"    "0xeec4ccd13fe05907f9d732a8ad245bcb7f918217157b89baaa23895c12eb329a" " --fmt" " api")

run_test("getTrans_first"              " 40500.0.next")
#run_test("getTrans_latest"             " latest")
run_test("getTrans_nav_prev1"          " 1001001.1.prev")
run_test("getTrans_nav_prev2"          " 1001001.0.prev")
run_test("getTrans_nav_next1"          " 1001001.2.next")
run_test("getTrans_nav_next2"          " 1001001.3.next")

run_test("getTrans_verbose_blk_idx1"   "--fmt" " json" " 0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.3")
run_test("getTrans_verbose_blk_idx2"   "--fmt" " api" " 0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.4")
run_test("getTrans_verbose_blk_idx3"   "0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.3" " -t")
run_test("getTrans_articulate"         "-a" " -x" " api" " -t" " 6513870.62" " 6687987.11" " 0x5f965cb0a6c3ea59454bc12cce0ed41017ec7effd2196f81f2fad45149f26e12")
run_test("getTrans_articulate_2"       "3648669.24" " -avtx" " json" " --api_mode")
run_test("getTrans_articulate_arrays"  "0xcf57ae0204729b4230034888a8508146c13ad75e8c9b584120b87cb20456650f" " -tax" " api")
run_test("getTrans_by_blk_hash_good"   "0x693b52d0fc09ec86d86aa117dd2b397116b6031cf2b672f8673c4441b9dc190f.55" " --api_mode")
# this test case should fail, but it doesn't. It calls for a transaction (#155) that does not exist since this block only has 94 transactions
#run_test("getTrans_by_blk_hash_bad"    "0x693b52d0fc09ec86d86aa117dd2b397116b6031cf2b672f8673c4441b9dc190f.155" " -v")

run_test("getTrans_three_ways"         "0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.79" " 3657480.79" " 0xda2f27fffb36c90c74fc53bd8b721074e87203c727640c95c4c802296e3bf8ad")
# this test case should fail but does not because if one has a --file: option, the rest of the options (other than the built-ins) are ignored
#run_test("getTrans_by_file"            "--bad --file:cmd_file")
run_test("getTrans_by_file"            "--file:cmd_file" " --fmt" " csv")
run_test("getTrans_by_file_long_v"     "--file:cmd_file" " --fmt" " json" " --parity")
run_test("getTrans_by_file_v"          "--fmt" " json" " --file:cmd_file")

run_test("getTrans_bad_hash_msg"         "0x060e4cf9fa8d34a8b423b5b3691b2541255ff7974ff16699e104edcfb63bd521" " 0x0766ec95bf12f9600afc6c33c287f9d2356d85f15f0d7eeeb6647f5d4ddfbc11" " 0x060e4cf9fa8d34a8b423b5b3691b2541255ff7974ff16699e104edcfb63bd521")
run_test("getTrans_bad_hash_msg_raw"     "--veryRaw" " 0x060e4cf9fa8d34a8b423b5b3691b2541255ff7974ff16699e104edcfb63bd521" " 0x0766ec95bf12f9600afc6c33c287f9d2356d85f15f0d7eeeb6647f5d4ddfbc11" " 0x060e4cf9fa8d34a8b423b5b3691b2541255ff7974ff16699e104edcfb63bd521")
run_test("getTrans_bad_blkhash_msg"      "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.1" " 0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.5" " 0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.2")
run_test("getTrans_bad_blkhash_msg_raw"  "--veryRaw" "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.1" " 0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.5")

run_test("getTrans_revert_not_err" "0x3551dac5e91df7f27c78118daa0da81211ad38acc527c08c74c5a7724397d49c" " -a" " --fmt" " json" " -t")

run_test("getTrans_message" "0xa152016a73c308e2ecdf9ff31d6a36f717604472a589f21e998e96c3e9500344" " -a" " --api_mode")
run_test("getTrans_core_dump_bug"      "0xa5c9c1a65f998be86dc2131d57d0de0c418bb534d8d2b37b46c99888ccef8d10" " -avtx" " api")

run_test("getTrans_fmt_txt"          "2222222.0" " 2332332.0" " --fmt" " txt")
run_test("getTrans_fmt_csv"          "2222222.0" " 2332332.0" " --fmt" " csv")
run_test("getTrans_fmt_json"         "2222222.0" " 2332332.0" " --fmt" " json")
run_test("getTrans_fmt_api"          "2222222.0" " 2332332.0" " --fmt" " api")
run_test("getTrans_fmt_api_mode"     "2222222.0" " 2332332.0" " --api_mode")
run_test("getTrans_fmt_api_mode_fmt" "2222222.0" " 2332332.0" " --api_mode" " --fmt" " txt")
run_test("getTrans_fmt_defaut"       "2222222.0" " 2332332.0")
run_test("getTrans_fmt_junk"         "2222222.0" " 2332332.0" " --fmt" " junk")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
