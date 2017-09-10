# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (printFloat)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin/test")

# Define the executable to be generated
set(TOOL_NAME "printFloat")
set(PROJ_NAME "libs/etherlib")
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
#
# There is no need to test the command line params on this code
#
run_test("printFloat_README" "-th")
run_test("printFloat_00")
run_test("printFloat_01" "-t:1")
run_test("printFloat_02" "-t:2")
run_test("printFloat_03" "-t:3")
run_test("printFloat_long_01" "--testNum:1")
run_test("printFloat_long_02" "--testNum:2")
run_test("printFloat_long_03" "--testNum:3")
run_test("printFloat_nc_01" "-t" " 1")
run_test("printFloat_nc_02" "-t" " 2")
run_test("printFloat_nc_03" "-t" " 3")
run_test("printFloat_nc_long_01" "--testNum" " 1")
run_test("printFloat_nc_long_02" "--testNum" " 2")
run_test("printFloat_nc_long_03" "--testNum" " 3")
