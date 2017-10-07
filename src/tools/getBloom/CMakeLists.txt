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
target_link_libraries (${TOOL_NAME} wallet token ${BASE_LIBS})

# Testing
# Define paths to test folder and gold folder
set(TEST_EXE "${EXECUTABLE_OUTPUT_PATH}/${TOOL_NAME}")
set(TEST_PATH "${TEST_PATH}/${PROJ_NAME}/${TOOL_NAME}")
set(GOLD_PATH "${GOLD_PATH}/${PROJ_NAME}/${TOOL_NAME}")

# Additional target to make the README.md
build_readme(${CMAKE_CURRENT_SOURCE_DIR} ${TOOL_NAME})

# Function to run an special or slow test case
function(run_special_test testName)
    run_the_special_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_special_test)

# Function to run an individual test case
function(run_test testName)
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_test)

# Enter one line for each individual test
run_test("getBloom_README"    "-th")
run_test("getBloom_no_params"      )
run_test("getBloom_empty"     "1001001")
run_test("getBloom_bloom"     "4000001")
run_test("getBloom_blooms"    "1590000" " 4000001" "4000002")
run_test("getBloom_range"     "4000001-4000003")
run_test("getBloom_baddata"   "1590000" " 4000001-4000002" " x400x001")

run_test("getBloom_combo_one_range" "1590000" " 4000001-4000002" " 1001001" " 1234567")
run_test("getBloom_combo_two_range" "1590000" " 4000001-4000002" " 1001001-1001100" " 1234567")

# display a bunch of blocks raw
run_test("getBloom_raw_01"       "--source:raw"   " 2768801" " 1995314" " 1958017")
run_test("getBloom_raw_02"       "-s:r"           " 2768801" " 1995314" " 1958017")
run_test("getBloom_raw_03"       "-s:raw"         " 2768801" " 1995314" " 1958017")
run_test("getBloom_raw_04"       "-r"             " 2768801" " 1995314" " 1958017")
run_test("getBloom_raw_05"       "--raw"          " 2768801" " 1995314" " 1958017")

# so that the test works, we must first bring these blocks onto the cache (uses undocumented options)
run_the_test(${TEST_PATH} ${GOLD_PATH} "getBloom_force_01" ${EXECUTABLE_OUTPUT_PATH}/getBlock " 2768801" " 1995314" " 1958017" " -o")

# display a bunch of blocks from cache
run_test("getBloom_cache_01"     "--source:cache" " 2768801" " 1995314" " 1958017" " -p")
run_test("getBloom_cache_02"     "-s:c"           " 2768801" " 1995314" " 1958017" " --parity")
run_test("getBloom_cache_03"     "-s:cache"       " 2768801" " 1995314" " 1958017" " -p")
run_test("getBloom_cache_04"     "-a"             " 2768801" " 1995314" " 1958017" " -p")

# now check them
#run_test("getBloom_check_01"     "2768801"        " -c")
#run_test("getBloom_check_02"     "1995314"        " --check")
#run_test("getBloom_check_03"     "1958017"        " -c")
#run_test("getBloom_chk_245001"   "245001"         " --check")

run_test("getBloom_parity_hex"   "--parity"       " 0x2e7721" " --receipt")
run_test("getBloom_parity_num"   "-p"             " 728708")
run_test("getBloom_parity_range" "-p"             " 358450-0x5785a")

run_test("getBloom_force_02"       " 2769609" " 2799895" " 2872831" " 3076260" " -o")
run_test("getBloom_multi1"   "-pr" " 2769609" " 2799895" " 2872831" " 3076260")
run_test("getBloom_multi2"   "-pa" " 2769609" " 2799895" " 2872831" " 3076260")

run_test("getBloom_short_receipt" "-e"        " 4001001")
run_test("getBloom_long_receipt"  "--receipt" " 4001001")

run_special_test("getBloom_cmd_file1" "--file:cmd_file1")
run_special_test("getBloom_cmd_file2" "--file:cmd_file2")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
