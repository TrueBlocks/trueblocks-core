# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (whereBlock)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "whereBlock")
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

# Function to run an special or slow test case
function(api_special_test testName)
    run_the_api_special_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(api_special_test)

# Function to run an individual test case
function(run_test testName)
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_test)

# Function to run an api mode test case
function(api_test testName)
    run_the_api_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(api_test)

# weird, but we have to make sure the block is in the cache first
run_the_test(${TEST_PATH} ${GOLD_PATH} "whereBlock_preload" ${EXECUTABLE_OUTPUT_PATH}/getBlock " 1001001" " -o")
run_the_test(${TEST_PATH} ${GOLD_PATH} "whereBlock_preload2" ${EXECUTABLE_OUTPUT_PATH}/getBlock " 1001010" " -o")

# Enter one line for each individual test
run_test("whereBlock_README"            "-th")
run_test("whereBlock_no_options")
run_test("whereBlock_invalid_option_1"  "-x")
run_test("whereBlock_invalid_option_2"  "--option")
run_test("whereBlock_invalid_option_3"  "-a" " -b")
run_test("whereBlock_help"              "-h")
run_test("whereBlock_long_help"         "--help")
run_test("whereBlock_notcached"         "45000")
run_test("whereBlock_cached"            "1001001")
run_test("whereBlock_blockByHash"       "0xd19bc98080cd70951fad3951cef2d512e6334aa3c7e64a38985c955d4d2e273b")

run_test("whereBlock_fmt_txt"           "1001001" " --fmt" " txt")
run_test("whereBlock_fmt_csv"           "1001001" " --fmt" " csv")
run_test("whereBlock_fmt_json"          "1001001" " --fmt" " json")
run_test("whereBlock_fmt_api"           "1001001" " --fmt" " api")
api_test("whereBlock_fmt_defaut"        "1001001")
api_test("whereBlock_fmt_api_mode"      "1001001")
api_test("whereBlock_fmt_api_mode_fmt"  "1001001" " --fmt" " txt")
run_test("whereBlock_fmt_junk"          "1001001" " --fmt" " junk")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
