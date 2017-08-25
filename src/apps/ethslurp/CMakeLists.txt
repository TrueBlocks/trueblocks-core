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
set(PROJ_NAME "apps")
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

# Function to run an individual test case
function(run_test testName)
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_test)

# Enter one line for each individual test
run_test("ethslurp_no_params"            )
run_test("ethslurp_help"                      "-h")
run_test("ethslurp_invalid_param_1"           "-x")
run_test("ethslurp_invalid_param_2"           "--")
run_test("ethslurp_invalid_address_1"         "invalid")
run_test("ethslurp_invalid_address_2"         "0x63c8c29af409bd31ec7ddeea58f")
run_test("ethslurp_not_found_address"         "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b5")
run_test("ethslurp_no_record_in_block_range"  "-b:0:1"             " 0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0")
run_test("ethslurp_single_block_range"        "-b:4186279:4186279" " 0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0")
run_test("ethslurp_range_of_blocks"           "-b:4186279:4186301" " 0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0")
# Long options not implemented yet
#run_test("ethslurp_no_record_in_block_range_long"  "--blocks:0:1" "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0")
#run_test("ethslurp_single_block_range_long"  "--blocks:4186279:4186279" "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0")
#run_test("ethslurp_range_of_blocks_long"  "--blocks:4186279:4186301" "0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0")
run_test("ethslurp_invalid_date_range_1"      "-d"                             " 0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0")
run_test("ethslurp_invalid_date_range_2"      "-da:b"                          " 0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0")
run_test("ethslurp_invalid_date_range_3"      "-d:a:b"                         " 0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0")
run_test("ethslurp_invalid_date_range_4"      "-d:201705191000"                " 0x1324e7b922b30B49Ab8EA81086341cc41C249346")
run_test("ethslurp_invalid_date_range_5"      "-d:20170519100000:201705190000" " 0x1324e7b922b30B49Ab8EA81086341cc41C249346")
run_test("ethslurp_no_record_in_dates_range"  "-d:20170720:20170820"           " 0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0")
run_test("ethslurp_single_date_range"         "-d:20170721:20170821"           " 0x63c8c29af409bd31ec7ddeea58ff14f21e8980b0")
run_test("ethslurp_date_range_1"              "-d:20170519:20170618"           " 0x1324e7b922b30B49Ab8EA81086341cc41C249346")
run_test("ethslurp_date_range_2"              "-d:20170519:20170530"           " 0x1324e7b922b30B49Ab8EA81086341cc41C249346")
run_test("ethslurp_in_tx_filter"              "-d:20170519:20170519"           " -i"  " 0x1324e7b922b30B49Ab8EA81086341cc41C249346")
run_test("ethslurp_out_tx_filter"             "-d:20170519:20170519"           " -e"  " 0x1324e7b922b30B49Ab8EA81086341cc41C249346")
# Pending bug here
#run_test("ethslurp_invalid_format"  " -d:20170519:20170519" " -f:invalid" " 0x1324e7b922b30B49Ab8EA81086341cc41C249346")
run_test("ethslurp_out_txt_format"            "-d:20170519:20170519" " -f:txt"  " 0x1324e7b922b30B49Ab8EA81086341cc41C249346")
run_test("ethslurp_out_csv_format"            "-d:20170519:20170519" " -f:csv"  " 0x1324e7b922b30B49Ab8EA81086341cc41C249346")
run_test("ethslurp_out_html_format"           "-d:20170519:20170519" " -f:html" " 0x1324e7b922b30B49Ab8EA81086341cc41C249346")
run_test("ethslurp_options_combination_1"     "-d:20170519:20170519" " -f:html" " -i" " -b:0" " 0x1324e7b922b30B49Ab8EA81086341cc41C249346")

run_test("ethslurp_options_combination_2"     "-f:csv" " -i"  " -b:3730547:3730547"       " 0x1324e7b922b30B49Ab8EA81086341cc41C249346")
run_test("ethslurp_verbose_options"           "-f:csv" " -i"  " -b:3730547:3730547" " -v" " 0x1324e7b922b30B49Ab8EA81086341cc41C249346")
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
#cat api_key | ethslurp -t                           >>proposed/test1.txt 2>&1
#find ~/.quickBlocks.test                            >>proposed/test1.txt 2>&1
#cat ~/.quickBlocks.test/quickBlocks.toml            >>proposed/test1.txt 2>&1

#--------------------------------------------------------------------------------
#echo "test2: slurp on known dead account"                                | tee proposed/test2.txt
#ethslurp -t 0x713B73c3994442b533e6A083eC968e40606810Ec                   >>proposed/test2.txt 2>&1
#cat ~/.quickBlocks.test/quickBlocks.toml | grep -v api_key               >>proposed/test2.txt 2>&1
#sleep 1s

#--------------------------------------------------------------------------------
#echo "test3: rerun flag"                                                 | tee proposed/test3.txt
#ethslurp -t -r                                                           >>proposed/test3.txt 2>&1

#--------------------------------------------------------------------------------
#echo "test4: test prettyPrint flag"                                      | tee proposed/test4.txt
#ethslurp -t -r -f                                                        >>proposed/test4.txt  2>&1; ./strip_confirms 4;
#sleep 1s

#--------------------------------------------------------------------------------
#echo "test5: test tab delimited flag"                                    | tee proposed/test5.txt
#ethslurp -t -v -r -f:txt                                                 >>proposed/test5.txt 2>&1

#--------------------------------------------------------------------------------
#echo "test6: test comma seperated value flag"                            | tee proposed/test6.csv
#ethslurp -t -v -r -f:csv                                                 >>proposed/test6.csv 2>&1
#sleep 1s

#--------------------------------------------------------------------------------
#echo "test7: test html flag"                                             | tee proposed/test7.html
#ethslurp -t -v -r -f:html                                                >>proposed/test7.html 2>&1

#--------------------------------------------------------------------------------
#echo "test8: extract income records only"                                | tee proposed/test8.txt
#ethslurp -t -v -i -f 0x682e426ea761db77bacd5acdce33ca122175daea          >>proposed/test8.txt 2>&1
#sleep 1s

#--------------------------------------------------------------------------------
#echo "test9: extract expense records only"                               | tee proposed/test9.txt
#ethslurp -t -v -e -f 0x682e426ea761db77bacd5acdce33ca122175daea          >>proposed/test9.txt 2>&1

#--------------------------------------------------------------------------------
#cat ~/.quickBlocks.test/quickBlocks.toml | grep -v fmt_fieldList >file
#mv -f file ~/.quickBlocks.test/quickBlocks.toml
#echo "fmt_fieldList=timestamp|date|from|nonce|value|ether|input|gasUsed|gas|gasPrice|blockNumber|transactionIndex|cumulativeGasUsed|blockHash|hash" >>~/.quickBlocks.test/quickBlocks.toml
#echo "test10: test date option fail"                                     | tee proposed/test10_fail.txt
#ethslurp -t -d -r -f:txt 0x713B73c3994442b533e6A083eC968e40606810Ec      >>proposed/test10_fail.txt 2>&1
#echo "test10: test date option less than"                                | tee proposed/test10_lessthan.txt
#ethslurp -t -d::20160519 -r -f:txt                                       >>proposed/test10_lessthan.txt 2>&1
#sleep 1s
#echo "test10: test date option greater than"                             | tee proposed/test10_greaterthan.txt
#ethslurp -t -d:20160510123300 -r -f:txt                                  >>proposed/test10_greaterthan.txt 2>&1
#echo "test10: test date option range"                                    | tee proposed/test10_range.txt


#--------------------------------------------------------------------------------
#cat ~/.quickBlocks.test/quickBlocks.toml | grep -v fmt_fieldList >file
#mv -f file ~/.quickBlocks.test/quickBlocks.toml
#echo "[display]"                               >~/.quickBlocks.test/configs/0x682e426ea761db77bacd5acdce33ca122175daea.toml
#echo "fmt_fieldList=from|value|ether|blockNumber" >>~/.quickBlocks.test/configs/0x682e426ea761db77bacd5acdce33ca122175daea.toml
#cat ~/.quickBlocks.test/quickBlocks.toml | grep -v api_key                >>proposed/test2.txt  2>&1
#cat ~/.quickBlocks.test/configs/*.toml | grep -v api_key                  >>proposed/test2.txt  2>&1
#ethslurp -t -d:20160510:20160519151605 -r -f:txt                          >>proposed/test10_range.txt  2>&1
#rm ~/.quickBlocks.test/configs/0x682e426ea761db77bacd5acdce33ca122175daea.toml


#--------------------------------------------------------------------------------
#echo "test11: test open config file"                                     | tee proposed/test11.txt
#ethslurp -t -o -r -f                                                     >>proposed/test11.txt  2>&1
#sleep 1s



#--------------------------------------------------------------------------------
#cat ~/.quickBlocks.test/quickBlocks.toml | grep -v fmt_fieldList >file
#mv -f file ~/.quickBlocks.test/quickBlocks.toml
#echo "test12: test block number option fail"                             | tee proposed/test12_fail.txt
#ethslurp -t -b -r -f                                                     >>proposed/test12_fail.txt  2>&1
#echo "test12: test block number option less than"                        | tee proposed/test12_lessthan.txt
#ethslurp -t -b::1491788 -r -f                                            >>proposed/test12_lessthan.txt  2>&1
#echo "test12: test block number option greater than"                     | tee proposed/test12_greaterthan.txt
#ethslurp -t -b:1491788 -r -f                                             >>proposed/test12_greaterthan.txt  2>&1
#sleep 1s
#echo "test12: test block number option range"                            | tee proposed/test12_range.txt
#ethslurp -t -b:1491752:1546617 -r -f                                     >>proposed/test12_range.txt 2>&1


#--------------------------------------------------------------------------------
#echo "fmt_custom_file=file:custom_format_file.html" >>~/.quickBlocks.test/quickBlocks.toml
#echo "fmt_custom_record=fmt_html_record" >>~/.quickBlocks.test/quickBlocks.toml
#echo "fmt_custom_field=fmt_html_field" >>~/.quickBlocks.test/quickBlocks.toml
#
#echo "test14: test custom display strings, file: format and fmt_ format" | tee proposed/test14.txt
#echo "[field_str]"    >>~/.quickBlocks.test/quickBlocks.toml
#echo "timestamp=date" >>~/.quickBlocks.test/quickBlocks.toml
#cat ~/.quickBlocks.test/quickBlocks.toml | grep -v api_key               >>proposed/test14.txt 2>&1
#ethslurp -t -r -f:custom                                                 >>proposed/test14.txt 2>&1


#--------------------------------------------------------------------------------
#echo "test16: test chunked commands (test,fmt,slurp,re-run)"             | tee proposed/test16.txt
#ethslurp -t -fr                                                          >>proposed/test16.txt 2>&1
#sleep 1s


#--------------------------------------------------------------------------------
#echo "test15: test list and cache clear options"                         | tee proposed/test15.txt
#ls ~/.quickBlocks.test/slurps                                            >>proposed/test15.txt 2>&1
#ethslurp -t -l                                                           >>proposed/test15.txt 2>&1
#ethslurp -t -c                                                           >>proposed/test15.txt 2>&1
#find ~/.quickBlocks.test                                                 >>proposed/test15.txt 2>&1


#--------------------------------------------------------------------------------
#echo "Bring down EEAR contract just to keep track of it"                      | tee proposed/eear.txt
#cat api_key | ethslurp -t -f:txt 0xbdace480953fc8260184bd625389ea20bbd0da9f   >>proposed/eear.txt 2>&1

