# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (ethslurp)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "ethslurp")
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

# Enter one line for each individual test
run_test("ethslurp_no_params")
run_test("ethslurp_help"                           "-h")
run_test("ethslurp_README"                         "-th")
run_test("ethslurp_invalid_param_1"                "-x")
run_test("ethslurp_invalid_param_2"                "--")
run_test("ethslurp_invalid_address_1"              "invalid")
run_test("ethslurp_invalid_address_2"              "0x63c8c29af409bd31ec7ddeea58f")
run_test("ethslurp_no_record_in_block_range"       "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0" " -b 0-1")
run_test("ethslurp_single_block_range"             "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0" " -b 4186279-4186279")
run_test("ethslurp_range_of_blocks"                "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0" " -b 4186279-4186301")
run_test("ethslurp_no_record_in_block_range_long"  "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0" " --blocks" " 0-1")
run_test("ethslurp_single_block_range_long"        "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0" " --blocks" " 4186279-4186279")
run_test("ethslurp_range_of_blocks_long"           "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0" " --blocks" " 4186279-4186301")
#run_test("ethslurp_invalid_date_range_1"           "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0" " -d" " 0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0")
#run_test("ethslurp_invalid_date_range_2"           "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0" " -da:b")
#run_test("ethslurp_invalid_date_range_3"           "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0" " -d:a:b")
#run_test("ethslurp_invalid_date_range_4"           "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -d:201705191000")
#run_test("ethslurp_invalid_date_range_5"           "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -d:20170519100000:201705190000")
#run_test("ethslurp_no_record_in_dates_range"       "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0" " -d:20170720:20170820")

#run_test("ethslurp_invalid_date_range_1_long"      "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0" " --dates" " 0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0")
#run_test("ethslurp_invalid_date_range_2_long"      "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0" " --datesa:b")
#run_test("ethslurp_invalid_date_range_3_long"      "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0" " --dates:a:b")
#run_test("ethslurp_invalid_date_range_4_long"      "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " --dates:201705191000")
#run_test("ethslurp_invalid_date_range_5_long"      "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " --dates:20170519100000:201705190000")
#run_test("ethslurp_no_record_in_dates_range_long"  "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0" " --dates:20170720:20170820")
#run_test("ethslurp_single_date_range_long"         "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0" " --dates:20170821:20170821")
#run_test("ethslurp_invalid_format"                 "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -d:20170519:20170519"      " -f:invalid")
#run_test("ethslurp_invalid_format_long"            "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -d:20170519:20170519"      " --fmt:invalid")
#run_test("ethslurp_options_combination_1"          "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -d:20170519:20170519"      " -f:html"    " -i"  " -b" " 0")
#run_test("ethslurp_options_combination_1_long"     "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " --dates:20170519:20170519" " --fmt:html" " --income" " --blocks" " 0")

run_test("ethslurp_invalid_type1"                   "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " --type" " junk")
run_test("ethslurp_invalid_type2"                   "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -t"     " junk")
run_test("ethslurp_type_ext"                        "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " --type" " ext")
run_test("ethslurp_type_int"                        "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " --type" " int")
run_test("ethslurp_type_token"                      "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " --type" " token" " --blocks" " 1428756-1428758")
run_test("ethslurp_type_miner"                      "0x05a56e2d52c817161883f50c441c3228cfe54d9f" " --type" " miner" " --blocks" " 1-1000")
run_test("ethslurp_type_both"                       "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " --type" " all")

run_special_test("ethslurp_not_found_address"              "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b5")
#run_special_test("ethslurp_date_range_1"                   "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -d:20170519:20170618")
#run_special_test("ethslurp_date_range_2"                   "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -d:20170519:20170530")
#run_special_test("ethslurp_date_range_1_long"              "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " --dates:20170519:20170618")
#run_special_test("ethslurp_date_range_2_long"              "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " --dates:20170519:20170530")
#run_special_test("ethslurp_in_tx_filter"                   "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -d:20170519:20170519"      " -i")
#run_special_test("ethslurp_out_tx_filter"                  "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -d:20170519:20170519"      " -e")
#run_special_test("ethslurp_in_tx_filter_long"              "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -d:20170519:20170519"      " --income")
#run_special_test("ethslurp_out_tx_filter_long"             "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -d:20170519:20170519"      " --expense")
#run_special_test("ethslurp_out_txt_format"                 "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -d:20170519:20170519"      " -f:txt")
#run_special_test("ethslurp_out_csv_format"                 "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -d:20170519:20170519"      " -f:csv")
##run_special_test("ethslurp_out_html_format"                "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -d:20170519:20170519"      " -f:html")
#run_special_test("ethslurp_out_txt_format_long"            "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -d:20170519:20170519"      " --fmt:txt")
#run_special_test("ethslurp_out_csv_format_long"            "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -d:20170519:20170519"      " --fmt:csv")
##run_special_test("ethslurp_out_html_format_long"           "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -d:20170519:20170519"      " --fmt:html")
#run_special_test("ethslurp_options_combination_2"          "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -b:3730547-3730548"        " -f:csv"     " -i")
#run_special_test("ethslurp_verbose_options"                "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " -b:3730547-3730548"        " -f:csv"     " -i"  " -v")
#run_special_test("ethslurp_options_combination_2_long"     "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " --blocks" " 3730547-3730548"  " --fmt:csv"  " --income")
#run_special_test("ethslurp_verbose_options_long"           "0x1324e7b922b30B49Ab8EA81086341cc41C249346" " --blocks" " 3730547-3730548"  " --fmt:csv"  " --income" " --verbose")
#run_special_test("ethslurp_single_date_range"              "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0" " -d:20170821:20170821")
# For the previous test case - set CACHE_ADDR at environment, this way we can clean the cache before running it
#set_property(TEST ethslurp_single_date_range PROPERTY ENVIRONMENT "CACHE_ADDR=0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0")


#--------------------------------------------------------------------------------
## clean up prior tests
#rm -fR ~/.quickBlocks.test

#--------------------------------------------------------------------------------
## prepare for these tests
#mkdir ~/.quickBlocks.test
#mkdir ~/.quickBlocks.test/configs
#mkdir ~/.quickBlocks.test/abis
#mkdir ~/.quickBlocks.test/makeClass

#--------------------------------------------------------------------------------
# show contents of testing folder
#find ~/.quickBlocks.test                             >proposed/test1.txt 2>&1
#echo "test1: default behaviour should show help"    >>proposed/test1.txt 2>&1
#note: we need an API key
#cat api_key | ethslurp --testing                    >>proposed/test1.txt 2>&1
#find ~/.quickBlocks.test                            >>proposed/test1.txt 2>&1
#cat ~/.quickBlocks.test/quickBlocks.toml            >>proposed/test1.txt 2>&1

#--------------------------------------------------------------------------------
#echo "test2: slurp on known dead account"                                | tee proposed/test2.txt
#ethslurp --testing 0x713B73c3994442b533e6A083eC968e40606810Ec            >>proposed/test2.txt 2>&1
#cat ~/.quickBlocks.test/quickBlocks.toml | grep -v api_key               >>proposed/test2.txt 2>&1
#sleep 1s

#--------------------------------------------------------------------------------
#echo "test3: rerun flag"                                                 | tee proposed/test3.txt
#ethslurp --testing -r                                                    >>proposed/test3.txt 2>&1

#--------------------------------------------------------------------------------
#echo "test4: test prettyPrint flag"                                      | tee proposed/test4.txt
#ethslurp --testing -r -f                                                 >>proposed/test4.txt  2>&1; ./strip_confirms 4;
#sleep 1s

#--------------------------------------------------------------------------------
#echo "test5: test tab delimited flag"                                    | tee proposed/test5.txt
#ethslurp --testing -v -r -f:txt                                          >>proposed/test5.txt 2>&1

#--------------------------------------------------------------------------------
#echo "test6: test comma seperated value flag"                            | tee proposed/test6.csv
#ethslurp --testing -v -r -f:csv                                          >>proposed/test6.csv 2>&1
#sleep 1s

#--------------------------------------------------------------------------------
#echo "test7: test html flag"                                             | tee proposed/test7.html
#ethslurp --testing -v -r -f:html                                         >>proposed/test7.html 2>&1

#--------------------------------------------------------------------------------
#echo "test8: extract income records only"                                | tee proposed/test8.txt
#ethslurp --testing -v -i -f 0x682e426ea761db77bacd5acdce33ca122175daea   >>proposed/test8.txt 2>&1
#sleep 1s

#--------------------------------------------------------------------------------
#echo "test9: extract expense records only"                               | tee proposed/test9.txt
#ethslurp --testing -v -e -f 0x682e426ea761db77bacd5acdce33ca122175daea   >>proposed/test9.txt 2>&1

#--------------------------------------------------------------------------------
#cat ~/.quickBlocks.test/quickBlocks.toml | grep -v fmt_fieldList >file
#mv -f file ~/.quickBlocks.test/quickBlocks.toml
#echo "fmt_fieldList=timestamp|date|from|nonce|value|ether|input|gasUsed|gas|gasPrice|blockNumber|transactionIndex|cumulativeGasUsed|blockHash|hash" >>~/.quickBlocks.test/quickBlocks.toml
#echo "test10: test date option fail"                                     | tee proposed/test10_fail.txt
#ethslurp --testing -d -r -f:txt 0x713B73c3994442b533e6A083eC968e40606810Ec >>proposed/test10_fail.txt 2>&1
#echo "test10: test date option less than"                                | tee proposed/test10_lessthan.txt
#ethslurp --testing -d::20160519 -r -f:txt                                >>proposed/test10_lessthan.txt 2>&1
#sleep 1s
#echo "test10: test date option greater than"                             | tee proposed/test10_greaterthan.txt
#ethslurp --testing -d:20160510123300 -r -f:txt                           >>proposed/test10_greaterthan.txt 2>&1
#echo "test10: test date option range"                                    | tee proposed/test10_range.txt


#--------------------------------------------------------------------------------
#cat ~/.quickBlocks.test/quickBlocks.toml | grep -v fmt_fieldList >file
#mv -f file ~/.quickBlocks.test/quickBlocks.toml
#echo "[display]"                               >~/.quickBlocks.test/configs/0x682e426ea761db77bacd5acdce33ca122175daea.toml
#echo "fmt_fieldList=from|value|ether|blockNumber" >>~/.quickBlocks.test/configs/0x682e426ea761db77bacd5acdce33ca122175daea.toml
#cat ~/.quickBlocks.test/quickBlocks.toml | grep -v api_key                >>proposed/test2.txt  2>&1
#cat ~/.quickBlocks.test/configs/*.toml | grep -v api_key                  >>proposed/test2.txt  2>&1
#ethslurp --testing -d:20160510:20160519151605 -r -f:txt                   >>proposed/test10_range.txt  2>&1
#rm ~/.quickBlocks.test/configs/0x682e426ea761db77bacd5acdce33ca122175daea.toml


#--------------------------------------------------------------------------------
#echo "test11: test open config file"                                     | tee proposed/test11.txt
#ethslurp --testing -o -r -f                                              >>proposed/test11.txt  2>&1
#sleep 1s



#--------------------------------------------------------------------------------
#cat ~/.quickBlocks.test/quickBlocks.toml | grep -v fmt_fieldList >file
#mv -f file ~/.quickBlocks.test/quickBlocks.toml
#echo "test12: test block number option fail"                             | tee proposed/test12_fail.txt
#ethslurp --testing -b -r -f                                              >>proposed/test12_fail.txt  2>&1
#echo "test12: test block number option less than"                        | tee proposed/test12_lessthan.txt
#ethslurp --testing -b 0-1491788 -r -f                                    >>proposed/test12_lessthan.txt  2>&1
#echo "test12: test block number option greater than"                     | tee proposed/test12_greaterthan.txt
#ethslurp --testing -b 1491788 -r -f                                      >>proposed/test12_greaterthan.txt  2>&1
#sleep 1s
#echo "test12: test block number option range"                            | tee proposed/test12_range.txt
#ethslurp --testing -b 1491752-1546617 -r -f                              >>proposed/test12_range.txt 2>&1


#--------------------------------------------------------------------------------
#echo "fmt_custom_file=file:custom_format_file.html" >>~/.quickBlocks.test/quickBlocks.toml
#echo "fmt_custom_record=fmt_html_record" >>~/.quickBlocks.test/quickBlocks.toml
#echo "fmt_custom_field=fmt_html_field" >>~/.quickBlocks.test/quickBlocks.toml
#
#echo "test14: test custom display strings, file: format and fmt_ format" | tee proposed/test14.txt
#echo "[field_str]"    >>~/.quickBlocks.test/quickBlocks.toml
#echo "timestamp=date" >>~/.quickBlocks.test/quickBlocks.toml
#cat ~/.quickBlocks.test/quickBlocks.toml | grep -v api_key               >>proposed/test14.txt 2>&1
#ethslurp --testing -r -f:custom                                          >>proposed/test14.txt 2>&1


#--------------------------------------------------------------------------------
#echo "test16: test chunked commands (test,fmt,slurp,re-run)"             | tee proposed/test16.txt
#ethslurp --testing -fr                                                   >>proposed/test16.txt 2>&1
#sleep 1s


#--------------------------------------------------------------------------------
#echo "test15: test list and cache clear options"                         | tee proposed/test15.txt
#ls ~/.quickBlocks.test/slu rps                                           >>proposed/test15.txt 2>&1
#ethslurp --testing -l                                                    >>proposed/test15.txt 2>&1
#ethslurp --testing -c                                                    >>proposed/test15.txt 2>&1
#find ~/.quickBlocks.test                                                 >>proposed/test15.txt 2>&1


#--------------------------------------------------------------------------------
#echo "Bring down EEAR contract just to keep track of it"                      | tee proposed/eear.txt
#cat api_key | ethslurp --testing -f:txt 0xbdace480953fc8260184bd625389ea20bbd0da9f >>proposed/eear.txt 2>&1

run_test("ethslurp_blockByHash" "0xd26114cd6ee289accf82350c8d8487fedb8a0c07" " --block" " 0xd19bc98080cd70951fad3951cef2d512e6334aa3c7e64a38985c955d4d2e273b")

# Installation steps, this tool is renamed at installation
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
install(CODE "file(RENAME \"${CMAKE_INSTALL_PREFIX}/bin/${TOOL_NAME}\" \"${CMAKE_INSTALL_PREFIX}/bin/ethSlurp\")")
