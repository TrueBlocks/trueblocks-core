# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (makeClass)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "makeClass")
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
set(README_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
add_custom_target(readme COMMAND ${TEST_EXE} "-th" ">" "${README_FILE}" "2>&1")

# Function to run an individual test case
function(run_test testName testArgs)
	run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${testArgs})
endfunction(run_test)

# Enter one line for each individual test
run_test("makeClass_README" "-th")
#broken run_test("makeClass_noparams1" "")
run_test("makeClass_noparams2" "x")
#broken run_test("makeClass_nonexist" "-e x")
#broken run_test("makeClass_conflict1" "-e -r")
#broken run_test("makeClass_conflict2" "-l -r")
#broken run_test("makeClass_conflict3" "-c -r")
#broken run_test("makeClass_conflict4" "-c -r -e")
run_test("makeClass_clear" "-c function")
#broken run_test("makeClass_list" "-l function")
#broken run_test("makeClass_edit" "-e function")
#broken run_test("makeClass_run" "-r function")
#broken cat function.[ch]* test11_code.cpp
#broken run_test("makeClass_run_ns" "-n:qblocks -r function")
#broken cat function.[ch]*
