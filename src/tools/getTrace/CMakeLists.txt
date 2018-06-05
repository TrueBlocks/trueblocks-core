# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (getTrace)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "getTrace")
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
run_the_test(${TEST_PATH} ${GOLD_PATH} "getTrace_preload" ${EXECUTABLE_OUTPUT_PATH}/getBlock " 3657480" " -o")

# Enter one line for each individual test

# Note: infura does not support tracing, so these 'help display' and 'error checking' are the only tests we can run regularly
run_test("getTrace_README"            "-th")
run_test("getTrace_help"               "-h")
run_test("getTrace_help_long"          "--help")
run_test("getTrace_no_input"          )
run_test("getTrace_bad_hash"          "0x0012")
run_test("getTrace_invalid_option_1"  "-p")
run_test("getTrace_invalid_option_2"  "--other")
run_test("getTrace_invalid_param_1"   ".th")
run_test("getTrace_invalid_param_2"   ".whatever")
run_test("getTrace_invalid_param_3"   "1233123")

# Note: the following tests will only work with a full archive tracing node
run_special_test("getTrace_by_txhash"          "0x66e49e99a984cd88e097782a001afaac8c6ff786c936651895475dd88ec360f6")
run_special_test("getTrace_by_bnun_and_txid"   "4348290.92")
run_special_test("getTrace_by_bhash_and_txid"  "0x96a09b25c3748974bb7e08780074482a164ab5a21e2ffc6c7115ec73c84e7f73.92" " -v" " --parity")
run_special_test("getTrace_from_node"          "-r" " 4000000.5")
run_special_test("getTrace_from_node_long"     "--raw" " 4000000.5")

run_special_test("getTrace_verbose_blk_idx1"   "-v" " 0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.3")
run_special_test("getTrace_verbose_blk_idx2"   "--verbose" " 0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.3")
run_special_test("getTrace_verbose_blk_idx3"   "0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.3" " -v")

run_special_test("getTrace_by_blk_hash_good"   "0x693b52d0fc09ec86d86aa117dd2b397116b6031cf2b672f8673c4441b9dc190f.55" " --verbose")
# this test case should fail, but it doesn't. It calls for a transaction (#155) that does not exist since this block only has 94 transactions
#run_special_test("getTrace_by_blk_hash_bad"    "0x693b52d0fc09ec86d86aa117dd2b397116b6031cf2b672f8673c4441b9dc190f.155" " -v")

run_special_test("getTrace_three_ways"         "0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.79" " -v" " 3657480.79" " 0xda2f27fffb36c90c74fc53bd8b721074e87203c727640c95c4c802296e3bf8ad")
# this test case should fail but does not because if one has a --file: option, the rest of the options (other than the built-ins) are ignored
#run_special_test("getTrace_by_file"            "--bad --file:cmd_file")
run_special_test("getTrace_by_file"            "--file:cmd_file")
run_special_test("getTrace_by_file_long_v"     "--file:cmd_file" " --verbose" " --parity")
run_special_test("getTrace_by_file_v"          "--verbose"       " --file:cmd_file")

run_special_test("getTrace_00" "0x0cc945824def6a5c81d86ca56848f92163838bbd480a3bd2ff85f6d5b13d72ab" "-r")
run_special_test("getTrace_01" "0x27fe46968ee62ec01ccb5dd3cbabd479f03c0374c2b6897c0faabfc748367b12" " 0x0cc945824def6a5c81d86ca56848f92163838bbd480a3bd2ff85f6d5b13d72ab" " --parity")
run_special_test("getTrace_02" "0x34a585645dcadff7c1003fcfdf193d82fd507d27f02097a441f867438da58b94")
run_special_test("getTrace_03" "0x17104ac9d3312d8c136b7f44d4b8b47852618065ebfa534bd2d3b5ef218ca1f3")
run_special_test("getTrace_04" "0x3d762e5f4c2a5c4ed3f4678d81f9078ff68e9d236d87dbcf3eee024dbb11f497" "-r")
run_special_test("getTrace_05" "0x409141e962166ceb967fa8cf8cded2394af088d6d270f1a0642c9bbfaf416348")
run_special_test("getTrace_06" "0x42a1ebc25c1a780052f70615bd23399f8300b4eae7e3431661867c47cbc12b08")
run_special_test("getTrace_07" "0x4e8b0b20a2314b11cdf6dd3e0ebae146b79bbe02fc64930ec59ed40c04d8364b")
run_special_test("getTrace_08" "0x590c575213aeec61ac15949f65b941ef2fb863479eb28a75d3a190278d88d1e7" " --parity")
run_special_test("getTrace_09" "0x71c72812a2564da07f5f06f0a130e593760140b9de4ee63f3e0b6758be549309")
run_special_test("getTrace_10" "0x79d7c345492ae4176b8ace43db41de69cf471b19df6421c6a570e961a9544e25")
run_special_test("getTrace_11" "0x7b467ef7d1336d060b3901e1d8afba50e3397e1ec66d8dfa82e89e8b250e9561" "-r")
run_special_test("getTrace_12" "0x8732bfb0c257c82c1d7964cd5f7b560b83b6ba3a32a808499dfd868633d115ad")
run_special_test("getTrace_13" "0x948a59164ab5f493074aed5345a1ff544bc1125dc0b265f17f032736fd145d22")
run_special_test("getTrace_14" "0x96d28d6a3b272a40a112e3248a2dd887c250f0ed8df736171f5861421ed77227" " -v")
run_special_test("getTrace_15" "0x99fd089739165e78fd568ec2aacd35627fcb34b1b7bcc5331fee0e83164e46dd")
run_special_test("getTrace_16" "0xa0205abafa7565ef328755bbf85c2588ec69c71da64703f05925b060838be15e")
run_special_test("getTrace_17" "0xa337336a2706c266340d8fb6c6da0725155d6ef61bbe62ca127356b7bf5ef7a6" "-r")
run_special_test("getTrace_18" "0xb4dc756ccfc28535de2eced46c5bc48f8a3c49a139e0f2064c0c7990adfd2e40")
run_special_test("getTrace_19" "0xb95dcd5f50036e79ad0a0102ad7af6919955fb6276aa63237c5c39dc9c0ba65c")
run_special_test("getTrace_20" "0xbc1922333f1a06d4ef263408354238ac9326b15401092951b002f12b9248f3f6")
run_special_test("getTrace_21" "0xbf2f0d64e3ea0e917b0578474f77f4fda3d90e662776ce0096100e75715076c7")
run_special_test("getTrace_22" "0xe024fde0ad85e7da742fc0f8d6b641756e2cb7c9d491f183b34c059f11a7f6ff")
run_special_test("getTrace_23" "0xeb888c721811e0a4c5d21ead63260f33cdd2780786ccc9f2e4c022823171d13f")
run_special_test("getTrace_24" "0xf0c6da6cac2a748129352d74db9c95b1278abe4945a49c2348c865e1c6039a02")

# The next two tests are of contract suicides. I used to have a bug that wouldn't record these.
# the contract suicided at this hash, my code had a bug that wouldn't record this properly
run_special_test("getTrace_25" "0x8324490c68e6268db9eadcb4ce8303ee1d256e480f7637853bb13a3ae1fe1328" "-r")
run_special_test("getTrace_26" "0x8324490c68e6268db9eadcb4ce8303ee1d256e480f7637853bb13a3ae1fe1328")

# the contract suicided at this hash, my code had a bug that wouldn't record this properly
run_special_test("getTrace_27" "0xf9d426284bd20415a53991a004122b3a3a619b295ea98d1d88a5fd3a4125408b" "-r")
run_special_test("getTrace_28" "0xf9d426284bd20415a53991a004122b3a3a619b295ea98d1d88a5fd3a4125408b")

run_test("getTrace_bad_hash_msg"                 "0x060e4cf9fa8d34a8b423b5b3691b2541255ff7974ff16699e104edcfb63bd521" " 0x0766ec95bf12f9600afc6c33c287f9d2356d85f15f0d7eeeb6647f5d4ddfbc11" " 0x060e4cf9fa8d34a8b423b5b3691b2541255ff7974ff16699e104edcfb63bd521")
run_test("getTrace_bad_hash_msg_raw"     "--raw" " 0x060e4cf9fa8d34a8b423b5b3691b2541255ff7974ff16699e104edcfb63bd521" " 0x0766ec95bf12f9600afc6c33c287f9d2356d85f15f0d7eeeb6647f5d4ddfbc11" " 0x060e4cf9fa8d34a8b423b5b3691b2541255ff7974ff16699e104edcfb63bd521")
run_test("getTrace_bad_blkhash_msg"              "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.1" " 0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.5")
run_test("getTrace_bad_blkhash_msg_raw"  "--raw" "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.1" " 0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.5")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
