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
target_link_libraries (${TOOL_NAME} ${BASE_LIBS})

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
run_test("getBlock_README"      "-th")
run_test("getBlock_no_params")
run_test("getBlock_invalid_option" "-s" " 1001001")
run_test("getBlock_empty"        "1001001")
run_test("getBlock_bloom"        "4000001")
run_test("getBlock_byhash"       "0x79990fd526c4751139a7a3afc7420cde1a1141b1920d2afd411858ecb4926a39")
run_test("getBlock_blooms"       "1590000"        " 4000001" " 4000002")
run_test("getBlock_range"        "4000001-4000003")
run_test("getBlock_baddata"      "1590000"        " 4000001-4000002" " x400x001")
run_test("getBlock_special_1"    "stateclear")
run_test("getBlock_special_2"    "daofork")

run_test("getBlock_combo_one_range" "1590000" " 4000001-4000002" " 1001001" " 1234567")
run_test("getBlock_combo_two_range" "1590000" " 4000001-4000002" " 1001001-1001100" " 1234567")

# display a bunch of blocks raw
run_test("getBlock_raw_01"       "--source:raw"   " 2768801" " 1995314" " 1958017" " --normalize")
run_test("getBlock_raw_02"       "-s:r"           " 2768801" " 1995314" " 1958017" " --normalize")
run_test("getBlock_raw_03"       "-s:raw"         " 2768801" " 1995314" " 1958017" " --normalize")
run_test("getBlock_raw_04"       "-r"             " 2768801" " 1995314" " 1958017" " --normalize")
run_test("getBlock_raw_05"       "--raw"          " 2768801" " 1995314" " 1958017" " --normalize")

# so that the test works, we must first bring these blocks onto the cache (uses undocumented options)
run_the_test(${TEST_PATH} ${GOLD_PATH} "getBlock_force_01" ${EXECUTABLE_OUTPUT_PATH}/getBlock " 2768801" " 1995314" " 1958017" " -o")

# display a bunch of blocks from cache
run_test("getBlock_cache_01"     "--source:cache" " 2768801" " 1995314" " 1958017" " --normalize" " --parity")
run_test("getBlock_cache_02"     "-s:c"           " 2768801" " 1995314" " 1958017" " --normalize" " --parity")
run_test("getBlock_cache_03"     "-s:cache"       " 2768801" " 1995314" " 1958017" " --normalize" " --parity")
run_test("getBlock_cache_04"     "-a"             " 2768801" " 1995314" " 1958017" " --normalize" " --parity")

# now check them
run_test("getBlock_check_01"     "2768801"        " -c")
run_test("getBlock_check_02"     "1995314"        " --check")
run_test("getBlock_check_03"     "1958017"        " -c")
run_test("getBlock_chk_245001"   "245001"         " --check")

run_test("getBlock_parity_hex"   "--parity"       " 0x2e7721")
run_test("getBlock_parity_num"   "--parity"       " 728708")
run_test("getBlock_parity_range" "--parity"       " 358450-0x5785a")

run_test("getBlock_force_02"                  " 2769609" " 2799895" " 2872831" " 3076260" " -o")
run_test("getBlock_multi1"   "--parity" " -r" " 2769609" " 2799895" " 2872831" " 3076260")
run_test("getBlock_multi2"   "--parity" " -a" " 2769609" " 2799895" " 2872831" " 3076260")

run_test("getBlock_terse_no_raw"  "-a"           " 4001001")
run_test("getBlock_terse_yes_raw" "-a" " -s:raw" " 4001001")

run_test("getBlock_cmd_file1" "--file:cmd_file1")
run_test("getBlock_cmd_file2" "--file:cmd_file2")

# More options
run_test("getBlock_help" "-h")
run_test("getBlock_help_long" "--help")

run_test("getBlock_byzantium_1" " 4369999" " 4370000" " 4380101")
run_test("getBlock_blockByHash" "0xd19bc98080cd70951fad3951cef2d512e6334aa3c7e64a38985c955d4d2e273b")

run_test("getBlock_latest1" "--latest")
run_test("getBlock_latest2" "--latest" " 0xd19bc98080cd70951fad3951cef2d512e6334aa3c7e64a38985c955d4d2e273b")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
