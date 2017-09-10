# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (grabABI)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "grabABI")
set(PROJ_NAME "apps")
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
run_test("grabABI_README"       "-th")
run_test("grabABI_verbose1"     "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " -v")
run_test("grabABI_verbose2"     "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " -v")
run_test("grabABI_noconst"      "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " -v" " -n")
run_test("grabABI_verbose3"     "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " -vn")
run_test("grabABI_encoding"     "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " -v" " -n" " -e")
run_test("grabABI_encoding_nv"  "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " -n" " -e")
run_test("grabABI_verbose4"     "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " -vne")
run_test("grabABI_generation"   "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " -v" " -n" " -e" " -g" " code")
run_test("grabABI_verbose5"     "0xbb9bc244d798123fde783fcc1c72d3bb8c189413" " -vng" " code")
run_test("grabABI_invalid_1"    "0x00001")
run_test("grabABI_not_found"    "-o" " 0x1728039ce0d18a799c081c5c7fa2090dd365a8d0")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
