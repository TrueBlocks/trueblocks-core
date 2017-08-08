# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (ethName)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "ethName")
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
# We set some variables, obtain the usage of the command (using add_custom_command())
# Then we build the output README.md file using the template + the command usage obtained
# readme target depends on the ${TOOL_HELP_CONTENT_FILE} so when needed it is executed first
set(README_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(TOOL_HELP_TAG "[{COMMAND_HELP}]")
set(TOOL_README_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/docs/README.tmpl")
set(TOOL_HELP_CONTENT_FILE "${CMAKE_CURRENT_BINARY_DIR}/help.txt")
add_custom_command(OUTPUT ${TOOL_HELP_CONTENT_FILE} COMMAND ${TOOL_NAME} "-h" ">" ${TOOL_HELP_CONTENT_FILE} "2>&1" DEPENDS ${TOOL_NAME})
add_custom_target(readme COMMAND "${TAG_REPLACE}" "${TOOL_README_TEMPLATE}" "${TOOL_HELP_TAG}" "${TOOL_HELP_CONTENT_FILE}" "${README_FILE}" DEPENDS ${TOOL_NAME} ${TOOL_HELP_CONTENT_FILE})
#add_custom_target(readme COMMAND ${TEST_EXE} "-th" ">" "${README_FILE}" "2>&1")

# Function to run an individual test case
function(run_test testName)
	run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGV1})
endfunction(run_test)

# Enter one line for each individual test
run_test("ethName_README" "-th")
run_test("ethName_no_params")
run_test("ethName_simple_search"  "0x1")
run_test("ethName_invalid_search" "0x1 rob")
run_test("ethName_three_params" "0x1 rob steem")
run_test("ethName_full_addr" "0xbf4ed7b27f1d666546e30d74d50d173d20bca754")
run_test("ethName_simple_w_space" "cur ")
run_test("ethName_w_source" "cur -s")
