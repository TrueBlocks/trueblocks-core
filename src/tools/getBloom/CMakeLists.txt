# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (getBloom)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "getBloom")
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

# Weird, but we have to call bloomMan here to seed the blooms into the cache so the test cases pass
#run_the_test(${TEST_PATH} ${GOLD_PATH} "getBloom_preload1" ${EXECUTABLE_OUTPUT_PATH}/bloomMan " --rewrite 1958017 1995314 2501030-2501060 2768801 2769609")
#run_the_test(${TEST_PATH} ${GOLD_PATH} "getBloom_preload2" ${EXECUTABLE_OUTPUT_PATH}/bloomMan " --rewrite 2799895 2872831 3076260 4501011 4501030-4501060")

# Enter one line for each individual test
run_test("getBloom_README"          "-th")
run_test("getBloom_no_params"       )
run_test("getBloom_empty"           "1001001")
run_test("getBloom_invalid_option"  "-x")
run_test("getBloom_bloom"           "4000001")
run_test("getBloom_blooms"          "1590000" " 4000001" "4000002")
run_test("getBloom_range"           "4000001-4000003")
run_test("getBloom_baddata"         "1590000" " 4000001-4000002" " x400x001")
run_test("getBloom_badopt5"         "4501030" " --receipts" " --block")
run_test("getBloom_badopt6"         "4501030" " -c" " --block")
run_test("getBloom_badopt7"         "4501030" " -c" " -b")
run_test("getBloom_badopt8"         "4501030" " --receipts" " -b")
run_test("getBloom_badopt9"         "4501030" " --bars" " --bits")
run_test("getBloom_badopt16"        "4501030" " --bars" " --bitbars")
run_test("getBloom_badopt17"        "4501030" " --bars" " --pctbars")
run_test("getBloom_badopt10"        "4501030" " -a" " --bits")
run_test("getBloom_badopt11"        "4501030" " -a" " -i")
run_test("getBloom_badopt12"        "4501030" " --bars" " -i")
run_test("getBloom_badopt14"        "4501030" " --eab" " --block")
run_test("getBloom_badopt15"        "4501030" " --eab" " --receipts" " --bitcount")

run_test("getBloom_rawlast1"        "4501030" " --eab" " --raw")
run_test("getBloom_rawlast2"        "4501030" " -e" " --raw")
run_test("getBloom_bitbars_raw"     "2501030-2501060" " --bitbars")
run_test("getBloom_pctbars_raw"     "2501030-2501060" " --pctbars")
run_test("getBloom_bars_block2"     "2501030-2501060" " --bitbars" " --block")
run_test("getBloom_bars_block3"     "2501030-2501060" " --pctbars" " --block")
run_test("getBloom_bars_receipt2"   "2501030-2501060" " --bitbars" " --receipts")
run_test("getBloom_bars_receipt3"   "2501030-2501060" " --pctbars" " --receipts")

run_test("getBloom_combo_one_range" "1590000 4000001-4000002 1001001 1234567 --receipts --verbose")
run_test("getBloom_combo_two_range" "1590000" " 4000001-4000002" " 1001001-1001100" " 1234567")

# display a bunch of blocks raw
run_test("getBloom_raw_01"          "--raw" " 2768801" " 1995314" " 1958017" "  --verbose")
run_test("getBloom_raw_02"          "--raw"    " 2768801" " 1995314" " 1958017")

# so that the test works, we must first bring these blooms onto the cache (uses undocumented options)
run_the_test(${TEST_PATH} ${GOLD_PATH} "getBloom_force_01" ${EXECUTABLE_OUTPUT_PATH}/getBlock " 2768801" " 1995314" " 1958017" " -o")

# display a bunch of blocks from cache
run_test("getBloom_parity_hex"      "--parity" " 0x2e7721" " --block")
run_test("getBloom_parity_num"      "--parity" " 728708")
run_test("getBloom_parity_range"    "--parity" " 358450-0x5785a" " --bitcount")
run_test("getBloom_multi1"          "--parity" " --raw" " 2769609" " 2799895" " 2872831" " 3076260")

run_test("getBloom_short_receipt"   "-c"         " 4001001")
run_test("getBloom_long_receipt"    "--receipts" " 4001001")

# Special blocks
run_test("getBloom_special_1"       "first")
run_test("getBloom_special_2"       "stateclear")

run_special_test("getBloom_cmd_file1" "--file:cmd_file1")
run_special_test("getBloom_cmd_file2" "--file:cmd_file2")

run_test("getBloom_blockByHash"
    "0xd19bc98080cd70951fad3951cef2d512e6334aa3c7e64a38985c955d4d2e273b"
    "0x5cfb2aeb032f8280c85f51857badb4cd3837427603f2939baa34692c8da52be4")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
