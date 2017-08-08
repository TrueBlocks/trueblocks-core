# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (getTrans)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "getTrans")
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

# Function to run an individual test case
function(run_test testName)
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGV1})
endfunction(run_test)

# Enter one line for each individual test
run_test("getTrans_README"     "-th")
run_test("getTrans_no_input"        )
run_test("getTrans_bad_hash"   "0x0012")

#broken run_test("getTrans_bad_index1"  "-v 0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.0")
run_test("getTrans_bad_index2"  "--verbose 0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.0")
run_test("getTrans_bad_index3"  "0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.0 -v")

run_test("getTrans_three_ways" "0x0f1217b92276cd17608d4212879739e6a5ec388bd7a03bef9798655234afd2b9.79 -v 3657480.79 0xda2f27fffb36c90c74fc53bd8b721074e87203c727640c95c4c802296e3bf8ad")
run_test("getTrans_by_file"    "--file:cmd_file")
#broken run_test("getTrans_by_file_v"  "--file:cmd_file --verbose")
