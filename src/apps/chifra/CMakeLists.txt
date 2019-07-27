# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (chifra)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "chifra")
set(PROJ_NAME "apps")
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

# Function to run an individual test case
function(run_test testName)
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_test)

# To run only specific tests we define a test configuration named "test_all"
add_custom_target(test-all COMMAND ${CMAKE_CTEST_COMMAND} -C test_all)

# Function to run an special or slow test case
function(run_special_test testName)
    run_the_special_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_special_test)

# Enter one line for each individual test
#run_test("chifra_README"             "-th")
#run_test("chifra_no_params")
#run_test("chifra_invalid_option"     "not_a_mode")

#run_test("chifra_config_noparms"      "config")
#run_test("chifra_config_show_noaddr"  "config" " show")
#run_test("chifra_config_show_badaddr" "config" " show" " 0x1234567812345678123456781234567812345678")
#run_test("chifra_config_show_tip"     "config" " show" " 0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359")
#run_test("chifra_config_edit"         "config" " edit" " 0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359")
#run_test("chifra_config_show_tip_v"   "config" " show" " 0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359" " --verbose:3")

#run_test("chifra_daemon_noparms"      "scrape" "--daemon")
#run_test("chifra_daemon_badparms"     "scrape" "--daemon" " --not_an_option")
#run_test("chifra_daemon"              "scrape" "--daemon" " --sleep 10" " --filter 0x1,0x2")

#run_test("chifra_scrape"           "scrape")

#run_test("chifra_ls_1"             "ls")
#run_test("chifra_ls_2"             "ls" " -l")
#run_test("chifra_ls_3"             "ls" " 0x1234567812345678123456781234567812345678")
#run_test("chifra_ls_4"             "ls" " -l" " 0x1234567812345678123456781234567812345678")
#run_test("chifra_ls_5"             "ls" " unused_param")

#run_test("chifra_stats"            "stats")
#run_test("chifra_seed"             "seed" " --verbose")
#run_test("chifra_freshen"          "freshen" " 0x1234567812345678123456781234567812345678")
#run_test("chifra_list"             "list" " 0x1234567812345678123456781234567812345678")
#run_test("chifra_slurp"            "slurp" " 0x1234567812345678123456781234567812345678")
#run_test("chifra_export"           "export" " 0x1234567812345678123456781234567812345678")
#run_test("chifra_init"             "init" " 0x1234567812345678123456781234567812345678")
#run_test("chifra_prices"           "prices" " ")

#run_test("chifra_accounts_noparams"    "accounts")
#run_test("chifra_accounts_badparams"   "accounts" " --not_an_option")
#run_test("chifra_accounts_edit"        "accounts" " edit")
#run_test("chifra_accounts"             "accounts" " -d")

run_test("chifra_blocks_noparams"    "blocks")
run_test("chifra_blocks_badparams"   "blocks" " --not_an_option")
run_test("chifra_blocks_edit"        "blocks" " edit")
run_test("chifra_blocks"             "blocks" " -d")

# Installation steps, this tool is renamed at installation
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
