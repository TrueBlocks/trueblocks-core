# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (getReceipt)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "getReceipt")
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

# Target to run only specific (long-running) test cases
add_custom_target(test-all COMMAND ${CMAKE_CTEST_COMMAND} -C test_all)
#add_custom_target(test-state COMMAND ${CMAKE_CTEST_COMMAND} -C test_state)

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

# Weird, but we have to call the getBlock code to make sure the block is in the cache
run_the_test(${TEST_PATH} ${GOLD_PATH} "getReceipt_preload" ${EXECUTABLE_OUTPUT_PATH}/getBlock " 3657480" " -o")

# Enter one line for each individual test
run_test("getReceipt_README"              "-th")
run_test("getReceipt_help"                "-h")
run_test("getReceipt_help_long"           "--help")
run_test("getReceipt_default"             "--address" " 0xbb9bc244d798123fde783fcc1c72d3bb8c189413")
run_test("getReceipt_no_input"            )
run_test("getReceipt_bad_hash"            "0x0012")

run_test("getReceipt_invalid_option_1"    "-p")
run_test("getReceipt_invalid_option_2"    "--other")
run_test("getReceipt_invalid_param_1"     ".th")
run_test("getReceipt_invalid_param_2"     ".whatever")
run_test("getReceipt_invalid_param_3"     "1233123")

run_test("getReceipt_three_ways"          "0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.79" " --fmt" " json" " 3657480.79" " 0xda2f27fffb36c90c74fc53bd8b721074e87203c727640c95c4c802296e3bf8ad")
run_test("getReceipt_by_filebad"          "--bad" " --file:cmd_file")
run_test("getReceipt_by_file"             "--file:cmd_file")
api_test("getReceipt_by_file_long_v"      "--file:cmd_file" " --verbose" " --parity")
run_test("getReceipt_by_file_v"           "--verbose"       " --file:cmd_file" " --fmt" " api")

run_test("getReceipt_verbose_blk_idx1"    "-v" " 0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.3" " --fmt" " json")
run_test("getReceipt_verbose_blk_idx2"    "--verbose" " 0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.3" " --fmt" " api")
api_test("getReceipt_verbose_blk_idx3"    "0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.3" " -v")

run_test("getReceipt_by_txhash"           "-x" " api" " 0x6afbe0f0ea3613edd6b84b71260836c03bddce81604f05c81a070cd671d3d765")
run_test("getReceipt_by_bnun_and_txid"    "4000000.5" " --logs")
run_test("getReceipt_by_bhash_and_txid"   "-x" "json" " 0xb8a3f7f5cfc1748f91a684f20fe89031202cbadcd15078c49b85ec2a57f43853.4" " -v" " --parity")

run_test("getReceipt_by_blk_hash_good"    "0x693b52d0fc09ec86d86aa117dd2b397116b6031cf2b672f8673c4441b9dc190f.55" " --verbose")
api_test("getReceipt_by_blk_hash_bad"     "0x693b52d0fc09ec86d86aa117dd2b397116b6031cf2b672f8673c4441b9dc190f.155")

run_test("getReceipt_from_node"           "--raw" " 4000000.5")
run_test("getReceipt_from_node_long"      "--raw" " 4000000.5")

run_test("getReceipt_bad_hash_msg"                 "0x060e4cf9fa8d34a8b423b5b3691b2541255ff7974ff16699e104edcfb63bd521" " 0x0766ec95bf12f9600afc6c33c287f9d2356d85f15f0d7eeeb6647f5d4ddfbc11" " 0x060e4cf9fa8d34a8b423b5b3691b2541255ff7974ff16699e104edcfb63bd521")
run_test("getReceipt_bad_hash_msg_raw"     "--raw" " 0x060e4cf9fa8d34a8b423b5b3691b2541255ff7974ff16699e104edcfb63bd521" " 0x0766ec95bf12f9600afc6c33c287f9d2356d85f15f0d7eeeb6647f5d4ddfbc11" " 0x060e4cf9fa8d34a8b423b5b3691b2541255ff7974ff16699e104edcfb63bd521")
run_test("getReceipt_bad_blkhash_msg"              "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.1" " 0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.5")
run_test("getReceipt_bad_blkhash_msg_raw"  "--raw" "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.1" " 0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.5")

run_test("getReceipt_prev_bug2_1"         "4279119.46")
run_test("getReceipt_prev_bug2_2"         "4279119.46" " --raw")

api_test("getReceipt_star"             "1428993.*")
run_test("getReceipt_fmt_txt"          "2222222.0" " 2332332.0" " --fmt" " txt")
run_test("getReceipt_fmt_csv"          "2222222.0" " 2332332.0" " --fmt" " csv")
run_test("getReceipt_fmt_json"         "2222222.0" " 2332332.0" " --fmt" " json")
run_test("getReceipt_fmt_api"          "2222222.0" " 2332332.0" " --fmt" " api")
api_test("getReceipt_fmt_api_mode"     "2222222.0" " 2332332.0")
api_test("getReceipt_fmt_api_mode_fmt" "2222222.0" " 2332332.0" " --fmt" " txt")
run_test("getReceipt_fmt_defaut"       "2222222.0" " 2332332.0")
run_test("getReceipt_fmt_junk"         "2222222.0" " 2332332.0" " --fmt" " junk")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
