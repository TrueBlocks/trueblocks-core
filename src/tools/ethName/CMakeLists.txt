# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (ethName)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "ethName")
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
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGV1})
endfunction(run_test)

# Enter one line for each individual test
run_test("ethName_no_params"            )
run_test("ethName_invalid_param_1" "-x" )
run_test("ethName_invalid_param_2" "-x rob" )
run_test("ethName_invalid_param_3" "-x 0xbf4ed7b27f1d666546e30d74d50d173d20bca754" )
run_test("ethName_invalid_param_4" "-x rob 0xbf4ed7b27f1d666546e30d74d50d173d20bca754" )
run_test("ethName_invalid_param_5" "-x -a 0xbf4ed7b27f1d666546e30d74d50d173d20bca754" )
run_test("ethName_invalid_param_6" "-t" )
run_test("ethName_invalid_param_7" "0x17 rob other")
run_test("ethName_README"          "-th")
run_test("ethName_address_match" "0x1")
run_test("ethName_address_no_match" "0xFA1")
run_test("ethName_name_match" "rob")
run_test("ethName_name_no_match" "rob_no_found")
run_test("ethName_name_and_addr_match" "0x17 rob")
run_test("ethName_name_and_addr_no_match" "0x01 rob")
run_test("ethName_name_show_addr_match" "-a rob")
run_test("ethName_name_show_addr_no_match" "-a no_rob")
run_test("ethName_addr_show_addr_match" "-a 0x17")
run_test("ethName_addr_show_addr_no_match" "-a 0xFA1")
run_test("ethName_addr_name_show_addr_match" "-a 0x17 rob")
run_test("ethName_addr_name_show_addr_no_match" "-a 0x01 rob")
run_test("ethName_name_count_match" "-c rob")
run_test("ethName_name_count_no_match" "-c no_rob")
run_test("ethName_addr_count_match" "-c 0x17")
run_test("ethName_addr_count_no_match" "-c 0xFA1")
run_test("ethName_addr_name_count_match" "-c 0x17 rob")
run_test("ethName_addr_name_count_no_match" "-c 0x01 rob")
run_test("ethName_name_case_match" "-m rob")
run_test("ethName_name_case_no_match" "-m roB")
run_test("ethName_addr_case_match" "-m 0xfA")
run_test("ethName_addr_case_no_match" "-m 0xFA")
run_test("ethName_addr_name_case_match_both" "-m 0xfa Kraken")
run_test("ethName_addr_name_case_match_name" "-m 0xFA rob")
run_test("ethName_addr_name_case_match_addr" "-m 0xfa roB")
run_test("ethName_addr_name_case_no_match" "-m 0xfA roB")
run_test("ethName_name_source_match" "-s rob")
run_test("ethName_name_source_no_match" "-s no_rob")
run_test("ethName_addr_source_match" "-s 0x17")
run_test("ethName_addr_source_no_match" "-s 0xFA1")
run_test("ethName_addr_name_source_match" "-s 0x17 rob")
run_test("ethName_addr_name_source_no_match" "-s 0x01 rob")
# Seems broken or not operative the -v option
#run_test("ethName_name_verbose_match" "-v rob")
#run_test("ethName_name_verbose_no_match" "-v no_rob")
#run_test("ethName_addr_verbose_match" "-v 0x17")
#run_test("ethName_addr_verbose_no_match" "-v 0xFA1")
#run_test("ethName_addr_name_verbose_match" "-v 0x17 rob")
#run_test("ethName_addr_name_verbose_no_match" "-v 0x01 rob")
run_test("ethName_edit" "-e")
# Interactive, to review how we can test this!
#run_test("ethName_" "-e")
run_test("ethName_" "-l")
run_test("ethName_duplicated_option_1" "-a -a -a -a 0x17 rob")
run_test("ethName_duplicated_option_2" "-c -c 0x17 rob")
run_test("ethName_mixed_options_with_count_1" "-a -s -c 0x17 rob")
run_test("ethName_mixed_options_with_count_2" "-c -s -a 0x17 rob")
run_test("ethName_mixed_options_with_list_1" "-l -s -a 0x17 rob")
run_test("ethName_mixed_options_with_list_2" "-c -l -a 0x17 rob")
run_test("ethName_mixed_options_with_edit_1" "-e -s -a 0x17 rob")
run_test("ethName_mixed_options_with_edit_2" "-c -e -a 0x17 rob")
run_test("ethName_all_options" "-a -m -v -c -s 0x17 rob")
#run_test("ethName_simple_search"   "0x1")
#run_test("ethName_invalid_search"  "0x1 rob")
#run_test("ethName_three_params"    "0x1 rob steem")
#run_test("ethName_full_addr"       "0xbf4ed7b27f1d666546e30d74d50d173d20bca754")
#run_test("ethName_simple_w_space"  "cur ")
#run_test("ethName_w_source"        "cur -s")
