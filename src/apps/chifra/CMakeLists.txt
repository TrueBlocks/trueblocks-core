cmake_minimum_required (VERSION 3.5)

# application project
project (chifra)

ADD_GO_INSTALLABLE_PROGRAM(chifra *.go ${BIN_DIR})

# Define the executable to be generated
set(TOOL_NAME "chifra")
set(PROJ_NAME "apps")

# Testing
# Define paths to test folder and gold folder
set(TEST_EXE "${EXECUTABLE_OUTPUT_PATH}/${TOOL_NAME}")
set(TEST_PATH "${TEST_PATH}/${PROJ_NAME}/${TOOL_NAME}")
set(GOLD_PATH "${GOLD_PATH}/${PROJ_NAME}/${TOOL_NAME}")
