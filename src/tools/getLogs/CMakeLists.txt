# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (getLogs)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "getLogs")
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

# Weird, but we have to call the getBlock code to make sure the block is in the cache
run_the_test(${TEST_PATH} ${GOLD_PATH} "getLogs_preload" ${EXECUTABLE_OUTPUT_PATH}/getBlock " 3657480" " -o")
run_the_test(${TEST_PATH} ${GOLD_PATH} "getLogs_preload_abi" ${EXECUTABLE_OUTPUT_PATH}/grabABI " 0x51ffc1b089392a5bb65bf24eaf04d07d0e6f88b5")

# Enter one line for each individual test
run_test("getLogs_README"              "-th")
run_test("getLogs_help"                "-h")
run_test("getLogs_help_long"           "--help")
run_test("getLogs_default"             "--address" " 0xbb9bc244d798123fde783fcc1c72d3bb8c189413")
run_test("getLogs_no_input"            )
run_test("getLogs_bad_hash"            "0x0012")

run_test("getLogs_invalid_option_1"    "-p")
run_test("getLogs_invalid_option_2"    "--other")
run_test("getLogs_invalid_param_1"     ".th")
run_test("getLogs_invalid_param_2"     ".whatever")
run_test("getLogs_invalid_param_3"     "1233123")

run_test("getLogs_block_range"         "1718400-1718420")
run_test("getLogs_block_range_addr"    "1718400-1718420" " 0xbb9bc244d798123fde783fcc1c72d3bb8c189413")

#------------------------------------------------------------------------------------------------
# eth_getLogs
# Returns an array of all logs matching a given filter object.
#------------------------------------------------------------------------------------------------
# Parameters
# Object - The filter options:
# fromBlock: QUANTITY|TAG - (optional, default: "latest") Integer block number, or "latest" for the last mined block or "pending", "earliest" for not yet mined transactions.
# toBlock: QUANTITY|TAG - (optional, default: "latest") Integer block number, or "latest" for the last mined block or "pending", "earliest" for not yet mined transactions.
# address: DATA|Array, 20 Bytes - (optional) Contract address or a list of addresses from which logs should originate.
# topics: Array of DATA, - (optional) Array of 32 Bytes DATA topics. Topics are order-dependent. Each topic can also be an array of DATA with "or" options.
# blockhash: DATA, 32 Bytes - (optional) With the addition of EIP-234 (Geth >= v1.8.13 or Parity >= v2.1.0), blockHash is a new filter option which restricts the logs
#   returned to the single block with the 32-byte hash blockHash. Using blockHash is equivalent to fromBlock = toBlock = the block number with hash blockHash. If
#   blockHash is present in the filter criteria, then neither fromBlock nor toBlock are allowed.
#------------------------------------------------------------------------------------------------
# Example Parameters
# params: [{
# "topics": ["0x000000000000000000000000a94f5374fce5edbc8e2a8697c15331677e6ebf0b"]
# }]
#------------------------------------------------------------------------------------------------
# Example
# curl -X POST --data '{"jsonrpc":"2.0","method":"eth_getLogs","params":[{"topics":["0x000000000000000000000000a94f5374fce5edbc8e2a8697c15331677e6ebf0b"]}],"id":74}'
#------------------------------------------------------------------------------------------------
# Returns
# Array - Array of log objects, or an empty array if nothing has changed since last poll.
#
# For filters created with eth_newBlockFilter the return are block hashes (DATA, 32 Bytes), e.g. ["0x3454645634534..."].
# For filters created with eth_newPendingTransactionFilter the return are transaction hashes (DATA, 32 Bytes), e.g. ["0x6345343454645..."].
# For filters created with eth_newFilter logs are objects with following params:
#
# removed: TAG - true when the log was removed, due to a chain reorganization. false if its a valid log.
# logIndex: QUANTITY - integer of the log index position in the block. null when its pending log.
# transactionIndex: QUANTITY - integer of the transactions index position log was created from. null when its pending log.
# transactionHash: DATA, 32 Bytes - hash of the transactions this log was created from. null when its pending log.
# blockHash: DATA, 32 Bytes - hash of the block where this log was in. null when its pending. null when its pending log.
# blockNumber: QUANTITY - the block number where this log was in. null when its pending. null when its pending log.
# address: DATA, 20 Bytes - address from which this log originated.
# data: DATA - contains the non-indexed arguments of the log.
# topics: Array of DATA - Array of 0 to 4 32 Bytes DATA of indexed log arguments. (In solidity: The first topic is the hash of the signature 
# of the event (e.g. Deposit(address,bytes32,uint256)), except you declared the event #with the anonymous specifier.)
#------------------------------------------------------------------------------------------------
# range and direct log queries
# queryRawLogs(string_q& results, uint64_t fromBlock, uint64_t toBlock, const address_t& addr, const CTopicArray& topics);
# queryRawLogs(string_q& results, uint64_t fromBlock, uint64_t toBlock, const address_t& addr) { CTopicArray unused2; return queryRawLogs(results, fromBlock, toBlock, addr, unused2); }
# queryRawLogs(string_q& results, uint64_t fromBlock, uint64_t toBlock) { address_t unused1; CTopicArray unused2; return queryRawLogs(results, fromBlock, toBlock, unused1, unused2); }
# queryRawLogs(string_q& results, const hash_t& hash, const address_t& addr, const CTopicArray& topics);
# queryRawLogs(string_q& results, const hash_t& hash, const address_t& addr) { CTopicArray unused2; return queryRawLogs(results, hash, addr, unused2); }
# queryRawLogs(string_q& results, const hash_t& hash) { address_t unused1; CTopicArray unused2; return queryRawLogs(results, hash, unused1, unused2); }

run_test("getLogs_three_ways"          "0xbed082f2f6fb5f91924df74eb79ac37ea9490e49dda0b29a01db27d171efb424" " 0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.36" " -v" " 3657480.36" " 1001010.12" " 0xbed082f2f6fb5f91924df74eb79ac37ea9490e49dda0b29a01db27d171efb422")
run_test("getLogs_redir_output"        " -v" " 3657480.79" " --output" " testing_file")
run_test("getLogs_by_filebad"          "--bad" " --file:cmd_file")
run_test("getLogs_by_file"             "--file:cmd_file")
run_test("getLogs_by_file_long_v"      "--file:cmd_file" " --verbose" " --parity" " --fmt" " json")
run_test("getLogs_by_file_v"           "--verbose" " --file:cmd_file" " --fmt" " json")

run_test("getLogs_verbose_blk_idx1"    "0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.36" " --fmt" " csv")
run_test("getLogs_verbose_blk_idx2"    "0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.36" " --verbose")
run_test("getLogs_verbose_blk_idx3"    "0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.36" " --fmt" " json")

run_test("getLogs_by_txhash"           "0x6afbe0f0ea3613edd6b84b71260836c03bddce81604f05c81a070cd671d3d765")
api_test("getLogs_by_bnun_and_txid"    "4000000.5")
run_test("getLogs_by_bhash_and_txid"   "0xb8a3f7f5cfc1748f91a684f20fe89031202cbadcd15078c49b85ec2a57f43853.4" " -x" " json" " --parity")

run_test("getLogs_by_blk_hash_good"    "0x693b52d0fc09ec86d86aa117dd2b397116b6031cf2b672f8673c4441b9dc190f.55" " --verbose")
run_test("getLogs_by_blk_hash_bad"     "0x693b52d0fc09ec86d86aa117dd2b397116b6031cf2b672f8673c4441b9dc190f.155" " -v")

run_test("getLogs_from_node"           "--raw" " 4000000.5")
run_test("getLogs_from_node_long"      "--raw" " 4000000.5")

run_test("getLogs_bad_hash_msg"                 "0x060e4cf9fa8d34a8b423b5b3691b2541255ff7974ff16699e104edcfb63bd521" " 0x0766ec95bf12f9600afc6c33c287f9d2356d85f15f0d7eeeb6647f5d4ddfbc11" " 0x060e4cf9fa8d34a8b423b5b3691b2541255ff7974ff16699e104edcfb63bd521")
run_test("getLogs_bad_hash_msg_raw"     "--raw" " 0x060e4cf9fa8d34a8b423b5b3691b2541255ff7974ff16699e104edcfb63bd521" " 0x0766ec95bf12f9600afc6c33c287f9d2356d85f15f0d7eeeb6647f5d4ddfbc11" " 0x060e4cf9fa8d34a8b423b5b3691b2541255ff7974ff16699e104edcfb63bd521")
run_test("getLogs_bad_blkhash_msg"              "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.1" " 0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.5")
run_test("getLogs_bad_blkhash_msg_raw"  "--raw" "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.1" " 0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.5")

run_test("getLogs_prev_bug2_1"         "4279119.46" " --fmt" " api" " --parity")
run_test("getLogs_prev_bug2_2"         "4279119.46" " --raw")

api_test("getLogs_star"             "1428993.*")
run_test("getLogs_fmt_txt"          "2222222.0" " 2332332.0" " --fmt" " txt")
run_test("getLogs_fmt_csv"          "2222222.0" " 2332332.0" " --fmt" " csv")
run_test("getLogs_fmt_json"         "2222222.0" " 2332332.0" " --fmt" " json")
run_test("getLogs_fmt_api"          "2222222.0" " 2332332.0" " --fmt" " api")
api_test("getLogs_fmt_api_mode"     "2222222.0" " 2332332.0")
api_test("getLogs_fmt_api_mode_fmt" "2222222.0" " 2332332.0" " --fmt" " txt")
run_test("getLogs_fmt_defaut"       "2222222.0" " 2332332.0")
run_test("getLogs_fmt_junk"         "2222222.0" " 2332332.0" " --fmt" " junk")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
