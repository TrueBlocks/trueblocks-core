# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (isContract)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "isContract")
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

# Function to run an individual test case
function(run_test testName)
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_test)

# Enter one line for each individual test

# Commented out old test cases while testing it again
run_test("isContract_noparams"          )
run_test("isContract_dash_d_1"     "0x000001f568875f378bf6d170b790967fe429c81a" " -d")
run_test("isContract_dash_d_2"     "0x01d04122aeccab138a47103b500f7b3d3b356a27" " -d")
run_test("isContract_dash_n_1"     "0x05f65ab88dc474a83a98994291c7a0abd13c9ae1" " -n")
run_test("isContract_dash_n_2"     "0x0a43edfe106d295e7c1e591a4b04b5598af9474c" " -n" " 0x0e7100e836150589b2be7030523d23c86a2093b5")
run_test("isContract_three_addrs"  "0x1267895780369b48a02781eb08d353e426e5f256" " 0x1492cdd605d91658f228eab14d8444b60e6da8df" " 0x17f8e4bd5a79f095d7b680317de6a617d202b235")
run_test("isContract_no_options_1" "0x1b725d9bcbd26765fe4836d850631037dfb37e0a")
run_test("isContract_no_options_2" "0x1dce4922337d44c8a187ce5bee12d26107340e4b")
run_test("isContract_no_options_3" "0x207d0c591119bc4f01a25b6849ad375f6ea6af43")
run_test("isContract_no_options_4" "0x237f29bbfd52c768a02980ea8d4d983a1d234edc")
run_test("isContract_verbose_1"    "0x26c4e381ffbdc825be9b986c3ba40525f0771e8d" "--verbose")
run_test("isContract_verbose_2"    "0x2935aa0a2d2fbb791622c29eb1c117b65b7a9085" "-v")
run_test("isContract_verbose_3"    "0x2c19c7f9ae8b751e37aeb2d93a699722395ae18f" "-v")
run_test("isContract_seven_addrs"  " 0x3003208e77edf3b088b122b5de3a6fc8c8ef679d" " 0x314159265dd8dbb310642f98f50c066173c1259b" " 0x3364ed250ea774146a0fbbc1da0ffa6a81514ca7" " 0x3682ae583f8c542ede42a9ca41105e5740b80d55" " 0x3cc0356217a8e03b94cf6a975683fc32db2080c4" " 0x40b9b889a21ff1534d018d71dc406122ebcf3f5a" " 0x437631e209736187b21090c0269e7a5f443811c3")
run_test("isContract_diff_more_2"  "0x4671ebe586199456ca28ac050cc9473cbac829eb" " -n" " 0x48b4cb193b587c6f2dab1a9123a7bd5e7d490ced" " 0x4bab4e253813b7c831b67d57f2b6589b9542fc41")
run_test("isContract_no_opts_1"          )
run_test("isContract_no_opts_2"   "0x513f22abc27e1f539dff98396acc78638dcd9c1a")
run_test("isContract_no_opts_3"   "0x5524c55fb03cf21f549444ccbecb664d0acad706")
run_test("isContract_dash_d_3"    "0x580114cbeb41c62f55ff55ca9e6f028ceb92ae91" " -d")
run_test("isContract_dash_d_4"    "0x655f01313bb55c04e8ed36a5098984f3f3de57bc" " -d")
run_test("isContract_dash_dn_1"   "0x6d805b5de59d3f1779e4180b8547bcf728ff91ea" " -dn")
run_test("isContract_dash_dn_2"   "-dn 0x705f04a4f1e7a1e23caea390b8903bd43d96e18e" " 0x73cbe96839b723bc913d10ba32b58fc476988a36")
run_test("isContract_multiple"    "0x7753a9d834844cfde5c211ec3912b49f0d8b8e11" " 0x79d94aae97baf2017c1cd39f3b406d60712d086f" " 0x7d319a4b48aa12ad04604a4f0d3792b9574feaad" " 0x80d9c4f6209044de069c545f68644419b520a02f")
run_test("isContract_from_file"   "--file:cmd_file")

run_test("isContract_invalid_option_1" "-x")
run_test("isContract_invalid_option_2" "--option")
run_test("isContract_invalid_addr_1" "invalid")
run_test("isContract_invalid_addr_2" "0x90e63c3d53e0ea496845b7a03ec7548b70014a923")
run_test("isContract_invalid_addr_3" "0xrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr")
run_test("isContract_false_result" "0x000001f568875f378bf6d170b790967fe429c81a")
run_test("isContract_true_result" "0xab7c74abc0c4d48d1bdad5dcb26153fc8780f83e")
# ./isContract 0x0117ef7fdb2a5814dff83c50fb799741904cd28d  (To review, this is reported as false and ethscan.io says it is a contract)
run_test("isContract_multiple_addr_1" "0xab7c74abc0c4d48d1bdad5dcb26153fc8780f83e" " 0x000001f568875f378bf6d170b790967fe429c81a"  " 0x000001f568875f378bf6d170b790967fe429c81a"  " 0xab7c74abc0c4d48d1bdad5dcb26153fc8780f83e")
run_test("isContract_multiple_addr_2" " 0x3003208e77edf3b088b122b5de3a6fc8c8ef679d" " 0x314159265dd8dbb310642f98f50c066173c1259b" " 0x3364ed250ea774146a0fbbc1da0ffa6a81514ca7" " 0x3682ae583f8c542ede42a9ca41105e5740b80d55" " 0x3cc0356217a8e03b94cf6a975683fc32db2080c4" " 0x5484438c9bb11deeb87de29d7bf83c5d71dfd000" " 0xb62ef4c58f3997424b0cceab28811633201706bc")
run_test("isContract_display_code_invalid_addr" "0xrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr" " -d")
run_test("isContract_display_code_1" "0x000001f568875f378bf6d170b790967fe429c81a" " -d")
run_test("isContract_display_code_2" "0xab7c74abc0c4d48d1bdad5dcb26153fc8780f83e" " -d")
run_test("isContract_display_code_3" "0xab7c74abc0c4d48d1bdad5dcb26153fc8780f83e" " 0x000001f568875f378bf6d170b790967fe429c81a"  " 0x000001f568875f378bf6d170b790967fe429c81a"  " 0xab7c74abc0c4d48d1bdad5dcb26153fc8780f83e" " -d")
run_test("isContract_display_code_4" " 0x3003208e77edf3b088b122b5de3a6fc8c8ef679d" " 0x314159265dd8dbb310642f98f50c066173c1259b" " 0x3364ed250ea774146a0fbbc1da0ffa6a81514ca7" " 0x3682ae583f8c542ede42a9ca41105e5740b80d55" " 0x3cc0356217a8e03b94cf6a975683fc32db2080c4" " 0x5484438c9bb11deeb87de29d7bf83c5d71dfd000" " 0xb62ef4c58f3997424b0cceab28811633201706bc" " -d")
run_test("isContract_display_code_5" " 0x3003208e77edf3b088b122b5de3a6fc8c8ef679d" " 0xab7c74abc0c4d48d1bdad5dcb26153fc8780f83e" " 0x3364ed250ea774146a0fbbc1da0ffa6a81514ca7" " 0xab7c74abc0c4d48d1bdad5dcb26153fc8780f83e" " 0x3cc0356217a8e03b94cf6a975683fc32db2080c4" " 0x5484438c9bb11deeb87de29d7bf83c5d71dfd000" " 0xb62ef4c58f3997424b0cceab28811633201706bc" " -d")
run_test("isContract_long_display_code_1" "0x000001f568875f378bf6d170b790967fe429c81a" " --display")
run_test("isContract_long_display_code_2" " 0x3003208e77edf3b088b122b5de3a6fc8c8ef679d" " 0xab7c74abc0c4d48d1bdad5dcb26153fc8780f83e" " 0x3364ed250ea774146a0fbbc1da0ffa6a81514ca7" " 0xab7c74abc0c4d48d1bdad5dcb26153fc8780f83e" " 0x3cc0356217a8e03b94cf6a975683fc32db2080c4" " 0x5484438c9bb11deeb87de29d7bf83c5d71dfd000" " 0xb62ef4c58f3997424b0cceab28811633201706bc" " --display")
run_test("isContract_diff_1" " -n" "0x26c4e381ffbdc825be9b986c3ba40525f0771e8d" " 0x26c4e381ffbdc825be9b986c3ba40525f0771e8d")
run_test("isContract_diff_2" " -n" " 0x3003208e77edf3b088b122b5de3a6fc8c8ef679d" " 0x3003208e77edf3b088b122b5de3a6fc8c8ef6794")
run_test("isContract_diff_3" " -n" " 0x3003208e77edf3b088b122b5de3a6fc8c8ef679d" " 0xab7c74abc0c4d48d1bdad5dcb26153fc8780f83e")
run_test("isContract_diff_invalid_addr_num_1" " -n" " 0x3003208e77edf3b088b122b5de3a6fc8c8ef679d" " 0xab7c74abc0c4d48d1bdad5dcb26153fc8780f83e" " 0xab7c74abc0c4d48d1bdad5dcb26153fc8780f83e")
run_test("isContract_diff_invalid_addr_num_2" " -n" " 0x3003208e77edf3b088b122b5de3a6fc8c8ef679d")
run_test("isContract_long_diff_1" "--nodiff" "0x26c4e381ffbdc825be9b986c3ba40525f0771e8d" " 0x26c4e381ffbdc825be9b986c3ba40525f0771e8d")
run_test("isContract_long_diff_2" "--nodiff" " 0x3003208e77edf3b088b122b5de3a6fc8c8ef679d" " 0x3003208e77edf3b088b122b5de3a6fc8c8ef6794")
run_test("isContract_help" "-h")
run_test("isContract_long_help" "--help")
run_test("isContract_README"       "-th")
run_test("isContract_combined_options_1" "--nodiff" " 0x26c4e381ffbdc825be9b986c3ba40525f0771e8d" " 0x26c4e381ffbdc825be9b986c3ba40525f0771e8d" " -d")
run_test("isContract_combined_options_2" " 0x26c4e381ffbdc825be9b986c3ba40525f0771e8d" " 0x26c4e381ffbdc825be9b986c3ba40525f0771e8d" " --verbose" " --display")
run_test("isContract_combined_options_3" "-dn" " 0x26c4e381ffbdc825be9b986c3ba40525f0771e8d" " 0x26c4e381ffbdc825be9b986c3ba40525f0771e8d")
