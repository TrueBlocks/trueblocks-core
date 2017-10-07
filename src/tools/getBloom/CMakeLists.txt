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

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
