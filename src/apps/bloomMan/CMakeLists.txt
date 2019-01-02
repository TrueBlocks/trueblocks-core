# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (bloomMan)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "bloomMan")
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

# Enter one line for each individual test
run_test("bloomMan_README"          "-th")
run_test("bloomMan_no_params"              )
run_test("bloomMan_invalid_param_1" "--junk" )
run_test("bloomMan_invalid_param_2" "-s" " --junk" )
run_test("bloomMan_noList"          "--stats")

# tests
run_test("bloomMan_raw"        "--raw"   " 3231401-3231402" )
run_test("bloomMan_blocks"     "4001001" " 4001002")
run_test("bloomMan_blocks_v1"  "4001001" " 4001002" " -v")
run_test("bloomMan_blocks_v2"  "4001001" " 4001002" " -v:2")
run_test("bloomMan_addrs_only" "0xec513881d33cfec3a961e214f24be49b177b4578" " 0x46ed26db3cae39217027552d589e53a47899b4ce")
run_test("bloomMan_addrs"      "4001001" " 0xec513881d33cfec3a961e214f24be49b177b4578")
run_test("bloomMan_addrs_v1"   "4001001" " 0xec513881d33cfec3a961e214f24be49b177b4578" " -v")
run_test("bloomMan_addrs_v2"   "4001001" " 0xec513881d33cfec3a961e214f24be49b177b4578" " -v:2")
run_test("bloomMan_addrs_v3"   "4001001" " 0xec513881d33cfec3a961e214f24be49b177b4578" " -v:3")
run_test("bloomMan_check"      "--check" " 4001001-4001003")
run_test("bloomMan_check_v1"   "--check" " 1001001" " -v")
run_test("bloomMan_check_v2"   "--check" " 1001002" " -v:2")
run_test("bloomMan_check_v3"   "--check" " 1001003" " -v:3")
run_test("bloomMan_stats"      "4001001-4001012" " -b" " 1" " --stats")
run_test("bloomMan_stats2"     "4001001-4001012" " -b" " 2" " --stats")
run_test("bloomMan_stats5"     "4001010-4001020" " -b" " 5" " --stats")

# Installation steps, this tool is renamed at installation
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
