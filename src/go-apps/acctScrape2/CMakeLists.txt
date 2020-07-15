# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# application project
project (acctScrape2)

ADD_GO_INSTALLABLE_PROGRAM(acctScrape2 *.go)

# Define the executable to be generated
set(TOOL_NAME "acctScrape2")
set(PROJ_NAME "go-apps")

# Testing
# Define paths to test folder and gold folder
set(TEST_EXE "${EXECUTABLE_OUTPUT_PATH}/${TOOL_NAME}")
set(TEST_PATH "${TEST_PATH}/${PROJ_NAME}/${TOOL_NAME}")
set(GOLD_PATH "${GOLD_PATH}/${PROJ_NAME}/${TOOL_NAME}")

# Additional target to make the README.md
build_readme(${CMAKE_CURRENT_SOURCE_DIR} ${TOOL_NAME})
