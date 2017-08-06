# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (abiTest)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/bin")

# Define the executable to be generated
set(TOOL_NAME "abiTest")
set(PROJ_NAME "libs/abilib")
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
run_test("abilibTest_00" "-th")
run_test("abilibTest_01" "ecoding")
run_test("abilibTest_02" "encoding")
run_test("abilibTest_03" "generation" "encoding")
run_test("abilibTest_04" "generation" "encoding" "bad")
