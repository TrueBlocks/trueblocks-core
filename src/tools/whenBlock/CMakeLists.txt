# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (whenBlock)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "whenBlock")
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
set(README_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
add_custom_target(readme COMMAND ${TEST_EXE} "-th" ">" "${README_FILE}" "2>&1")

function(run_test testName testArgs)
    run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${testArgs})
endfunction(run_test)

# Enter one line for each individual test
run_test("whenBlock_00" "-th")
#broken -- HOW CAN I CAT THE CONTENTS OF THE FILES INTO THE TEST RESULT?
#run_test("whenBlock_01" ";cat blocks.txt dates.txt")
run_test("whenBlock_02" "--file:blocks.txt")
run_test("whenBlock_03" "--file:dates.txt")
