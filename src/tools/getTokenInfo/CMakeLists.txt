# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (getTokenInfo)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "getTokenInfo")
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
add_custom_target(test-state COMMAND ${CMAKE_CTEST_COMMAND} -C test_state)

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
set (BAD1 "0x5e44c3e467a49c9ca0296a9f130fc433041aaa2")
set (BAD2 "0xd26114cd6ee289accf82350c8d8487fedb8a0c0")
set (BITREX "0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98")
set (BLKHASH1 "0x45205b0bdb6c9cc9bf0eb3753ae98175c563389b41b2d4a2178593a3415772c1")
set (BLKHASH2 "0x966c946dff292e4e5d91169f3007c3c28cd50ca614b18db601806c486f9e6e87")

run_the_test(${TEST_PATH} ${GOLD_PATH} "getTokenInfo_preload_abi"  ${EXECUTABLE_OUTPUT_PATH}/grabABI " ${OMG}")
run_the_test(${TEST_PATH} ${GOLD_PATH} "getTokenInfo_preload_abi2" ${EXECUTABLE_OUTPUT_PATH}/grabABI " ${SWT}")

# Enter one line for each individual test
run_test("getTokenInfo_README"                             "-th")
run_test("getTokenInfo_noparams"                                )
run_test("getTokenInfo_help"                                "-h")
run_test("getTokenInfo_help_long"                       "--help")
run_test("getTokenInfo_help_verbose"              "--help" " -v")

run_test("getTokenInfo_invalid_params_1"                    "a" " b" " c")
run_test("getTokenInfo_invalid_params_2"                    "${OMG}" " 21321321312321")
run_test("getTokenInfo_invalid_params_3"                    "21321321312321" " ${OMG}" )

run_test("getTokenInfo_invalid_token"                       "${BAD2}" " ${EA1}")
run_test("getTokenInfo_invalid_addr"                        "${OMG}" " ${BAD1}")
run_test("getTokenInfo_token_does_not_exist"                "${NON}" " ${EA1}" " 4000000")
run_test("getTokenInfo_acccount_does_not_exist"             "${OMG}" " ${NN1}" " 4000000")
run_test("getTokenInfo_single_account"                      "${OMG}" " ${EA1}" " 4000000")
run_test("getTokenInfo_multiple_token_accounts"             "${OMG}" " ${EA1}" " ${OMG}" " 4000000")
run_test("getTokenInfo_block_out_of_range"                  "${OMG}" " ${EA1}" " ${BITREX}" " 16000000" )

run_test("getTokenInfo_by_account_table"                    "${SWT}" " ${EA2}"    " ${EA3}"      " --byAcct" " 4000000")
run_test("getTokenInfo_invalid_dollars"                     "${SWT}" " ${BITREX}" " ${EA2}"      " ${EA3}"   " 4285364" " 3510999" " 2510010" " --dollars")
run_test("getTokenInfo_special_1"                           "${OMG}" " ${EA1}"    " ${BITREX}"   " first")
run_test("getTokenInfo_special_2"                           "${OMG}" " ${EA1}"    " ${BITREX}"   " stateclear")
run_test("getTokenInfo_special_3"                           "${OMG}" " ${EA1}"    " ${BITREX}"   " stateclear" " daofund")

run_state_test("getTokenInfo_multiple_single_block"         "${OMG}" " ${EA1}"    " ${BITREX}"   " 4150000")
run_state_test("getTokenInfo_multiple_multiblock"           "${OMG}" " ${EA1}"    " ${BITREX}"   " 4141000"  " 4151000" )
run_state_test("getTokenInfo_by_account"                    "${SWT}" " ${EA3}"    " -b"          " 4000000")
run_state_test("getTokenInfo_by_account_long"               "${SWT}" " ${EA3}"    " --byAcct"    " 4000000")
run_state_test("getTokenInfo_by_account_long_nozero"        "${SWT}" " ${EA3}"    " --byAcct"    " -n"       " 4000000")
run_state_test("getTokenInfo_by_account_long_nozero_long"   "${SWT}" " ${EA3}"    " --byAcct"    " --nozero" " 4000000")
run_state_test("getTokenInfo_by_account_table_long"         "${SWT}" " ${EA3}"    " --byAcct"    " 4000000")
run_state_test("getTokenInfo_by_account_table_verbose"      "${SWT}" " ${EA3}"    " --byAcct"    " 4000000" " -v")
run_state_test("getTokenInfo_by_account_table_verbose_long" "${SWT}" " ${EA3}"    " --byAcct"    " 4000000" " --verbose")
run_state_test("getTokenInfo_top_three_swarm1"              "${SWT}" " ${EA3}"    " 4285364"     " 3510999"  " 2510010" "--byAcct")
run_state_test("getTokenInfo_top_three_swarm2"              "${SWT}" " ${BITREX}" " ${EA2}"      " ${EA3}"   " 4285364" " 3510999" " 2510010")
run_state_test("getTokenInfo_top_three_swarm3"              "${SWT}" " ${BITREX}" " ${EA2}"      " ${EA3}"   " 4285364" " 3510999" " 2510010")
run_state_test("getTokenInfo_wei"                           "${OMG}" " ${EA1}"    " ${BITREX}"   " 4150000"  " --wei")
run_state_test("getTokenInfo_special_and_block_combination" "${OMG}" " ${EA1}"    " ${BITREX}"   " 4225341"  "  stateclear" " daofund")
run_state_test("getTokenInfo_blockByHash"                   "${OMG}" " ${NET}"    " ${BLKHASH1}" " ${BLKHASH2}")

run_state_test("getTokenInfo_fmt_txt"                       "${EA4}" " --fmt" " txt"  " 2000000")
run_state_test("getTokenInfo_fmt_csv"                       "${EA4}" " --fmt" " csv"  " 2000000")
run_state_test("getTokenInfo_fmt_json"                      "${EA4}" " --fmt" " json" " 2000000" " --dollars")
run_state_test("getTokenInfo_fmt_api"                       "${EA4}" " --fmt" " api"  " 2000000")
run_state_test("getTokenInfo_fmt_junk"                      "${EA4}" " --fmt" " junk" " 2000000")
api_state_test("getTokenInfo_fmt_api_mode"                  "${EA4}" " ${NET}" " 2000000")

run_state_test("getTokenInfo_mode_fail"                     "${TIP}" " --mode" " junk"    " 2500000")
run_state_test("getTokenInfo_bal"                           "${TIP}" " --mode" " bal"     " 2500000")
run_state_test("getTokenInfo_nonce"                         "${TIP}" " --mode" " nonce"   " 2500000")
run_state_test("getTokenInfo_code"                          "${TIP}" " --mode" " code"              )
run_state_test("getTokenInfo_storage"                       "${TIP}" " --mode" " storage" " 2500000")
run_state_test("getTokenInfo_deployed"                      "${TIP}" " --mode" " deployed"          )
run_state_test("getTokenInfo_accttype"                      "${TIP}" " --mode" " accttype"          )
run_state_test("getTokenInfo_some"                          "${TIP}" " --mode" " some"    " 2500000")
run_state_test("getTokenInfo_all"                           "${TIP}" " --mode" " all"     " 2500000")
run_state_test("getTokenInfo_none"                          "${TIP}" " --mode" " none"    " 2500000")
run_state_test("getTokenInfo_nonce_only"                    "${TIP}" " --mode" " none" " --mode" " nonce" " 2500000")

run_test("getTokenInfo_info_name"                           "${OMG}" " --info " " name")
run_test("getTokenInfo_info_totalSupply"                    "${OMG}" " --info " " totalSupply")
run_test("getTokenInfo_info_decimals"                       "${OMG}" " --info " " decimals")
run_test("getTokenInfo_info_version"                        "${OMG}" " --info " " version")
run_test("getTokenInfo_info_symbol"                         "${OMG}" " --info " " symbol")
run_test("getTokenInfo_info_all"                            "${OMG}" " --info " " all")

run_state_test("getTokenInfo_info_name2"                    "${OMG}" " ${SWT}" " --info " " name")
run_state_test("getTokenInfo_info_totalSupply2"             "${OMG}" " ${SWT}" " --info " " totalSupply")
run_state_test("getTokenInfo_info_decimals2"                "${OMG}" " ${SWT}" " --info " " decimals")
run_state_test("getTokenInfo_info_version2"                 "${OMG}" " ${SWT}" " --info " " version")
run_state_test("getTokenInfo_info_symbol2"                  "${OMG}" " ${SWT}" " --info " " symbol")
run_state_test("getTokenInfo_info_all2"                     "${OMG}" " ${SWT}" " --info " " all")

run_state_test("getTokenInfo_info_name3"                    "${OMG}" " ${SWT}" " --info " " name" "        4001001-4001002")
run_state_test("getTokenInfo_info_totalSupply3"             "${OMG}" " ${SWT}" " --info " " totalSupply" " 4001001-4001002")
run_state_test("getTokenInfo_info_decimals3"                "${OMG}" " ${SWT}" " --info " " decimals" "    4001001-4001002")
run_state_test("getTokenInfo_info_version3"                 "${OMG}" " ${SWT}" " --info " " version" "     4001001-4001002")
run_state_test("getTokenInfo_info_symbol3"                  "${OMG}" " ${SWT}" " --info " " symbol" "      4001001-4001002")
run_state_test("getTokenInfo_info_all3"                     "${OMG}" " ${SWT}" " --info " " all" "         4001001-4001002")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
