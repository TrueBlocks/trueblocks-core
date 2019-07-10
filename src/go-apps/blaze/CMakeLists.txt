# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (blaze)

# GO_GET(leveldb github.com/syndtr/goleveldb/leveldb)
ADD_GO_INSTALLABLE_PROGRAM(blaze *.go)

# Define the executable to be generated
set(TOOL_NAME "blaze")
set(PROJ_NAME "go-apps")

# Testing
# Define paths to test folder and gold folder
set(TEST_EXE "${EXECUTABLE_OUTPUT_PATH}/${TOOL_NAME}")
set(TEST_PATH "${TEST_PATH}/${PROJ_NAME}/${TOOL_NAME}")
set(GOLD_PATH "${GOLD_PATH}/${PROJ_NAME}/${TOOL_NAME}")

# Additional target to make the README.md
build_readme(${CMAKE_CURRENT_SOURCE_DIR} ${TOOL_NAME})

# Function to run an individual test case
function(run_test testName)
#    message(MESSAGE " TEST_PATH: " ${TEST_PATH})
#    message(MESSAGE " GOLD_PATH: "  ${GOLD_PATH})
#    message(MESSAGE " testName: " ${testName})
#    message(MESSAGE " TEST_EXE: " ${TEST_EXE})
#    message(MESSAGE " GOPATH: " ${GO_PATH})
    run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_test)

run_test("blaze" "none")
