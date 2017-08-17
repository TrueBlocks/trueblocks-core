# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (jsonTest)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/bin")

# Define the executable to be generated
set(TOOL_NAME "gTest")
set(PROJ_NAME "libs/utillib")
add_executable(${TOOL_NAME} ${SOURCE_FILES})

# Add the project static libs at linking
target_link_libraries (${TOOL_NAME} ${BASE_LIBS} ${GTEST_LIBRARIES} pthread)

# Testing
# Define paths to test folder and gold folder
set(TEST_EXE "${EXECUTABLE_OUTPUT_PATH}/${TOOL_NAME}")
set(TEST_PATH "${TEST_PATH}/${PROJ_NAME}/${TOOL_NAME}")
set(GOLD_PATH "${GOLD_PATH}/${PROJ_NAME}/${TOOL_NAME}")

# Additional target to make the README.md
build_readme(${CMAKE_CURRENT_SOURCE_DIR} ${TOOL_NAME})

# Function to run an individual test case
#function(run_test testName)
#     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
#endfunction(run_test)

# Enter one line for each individual test
#run_test("jsonTest_nothing")
#run_test("jsonTest_test00"  "test.json")
#run_test("jsonTest_test01"  "test2.json")
#run_test("jsonTest_test02"  "test.json" " test2.json")

# to run now the test we define it
add_test(NAME ${TOOL_NAME} COMMAND ${TEST_EXE})
