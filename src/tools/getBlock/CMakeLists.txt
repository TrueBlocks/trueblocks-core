# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (getBlock)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "getBlock")
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
run_test("getBlock_README"      "-th")
run_test("getBlock_no_params")
run_test("getBlock_empty"        "1001001")
run_test("getBlock_bloom"        "4000001")
run_test("getBlock_blooms"       "1590000"        " 4000001" " 4000002")
run_test("getBlock_range"        "4000001-4000003")
run_test("getBlock_baddata"      "1590000"        " 4000001-400002")

# display a bunch of blocks raw
run_test("getBlock_raw_01"       "--source:raw"   " 2768801" " 1995314" " 1958017" " --normalize")
run_test("getBlock_raw_02"       "-s:r"           " 2768801" " 1995314" " 1958017" " --normalize")
run_test("getBlock_raw_03"       "-s:raw"         " 2768801" " 1995314" " 1958017" " --normalize")
run_test("getBlock_raw_04"       "-r"             " 2768801" " 1995314" " 1958017" " --normalize")

# display a bunch of blocks from cache
run_test("getBlock_cache_01"     "--source:cache" " 2768801" " 1995314" " 1958017" " --normalize" " -p")
run_test("getBlock_cache_02"     "-s:c"           " 2768801" " 1995314" " 1958017" " --normalize" " -p")
run_test("getBlock_cache_03"     "-s:cache"       " 2768801" " 1995314" " 1958017" " --normalize" " -p")
run_test("getBlock_cache_04"     "-c"             " 2768801" " 1995314" " 1958017" " --normalize" " -p")

# now check them
run_test("getBlock_check_01"     "2768801"        " -k")
run_test("getBlock_check_02"     "1995314"        " -k")
run_test("getBlock_check_03"     "1958017"        " -k")
run_test("getBlock_chk_245001"   "245001"         " -k")

run_test("getBlock_parity_hex"   "--parity"       " 0x2e7721")
run_test("getBlock_parity_num"   "-p"             " 728708")
run_test("getBlock_parity_range" "-p"             " 358450-0x5785a")

run_test("getBlock_multi1" "-pr" " 2769609" " 2799895" " 2872831" " 3076260")
run_test("getBlock_multi2" "-pc" " 2769609" " 2799895" " 2872831" " 3076260")

run_test("getBlock_terse_no_raw" "-t"  " 4001001")
run_test("getBlock_terse_yes_raw" "-t"  " -s:raw" " 4001001")

run_test("getBlock_cmd_file1" "--file:cmd_file1")
run_test("getBlock_cmd_file2" "--file:cmd_file2")
