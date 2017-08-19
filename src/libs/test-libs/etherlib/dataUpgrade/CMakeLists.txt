# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (dataUpgrade)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin/test")

# Define the executable to be generated
set(TOOL_NAME "dataUpgrade")
set(PROJ_NAME "libs/etherlib")
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
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_test)

# Enter one line for each individual test
run_test("dataUpgradeTest_README")
run_test("dataUpgradeTest_00" "0")
run_test("dataUpgradeTest_01" "1")
run_test("dataUpgradeTest_02" "2")
run_test("dataUpgradeTest_03" "3")
run_test("dataUpgradeTest_04" "--file:cmd_file")

run_test("dataUpgradeTest_CAbi"             "CAbi")
run_test("dataUpgradeTest_CFunction"        "CFunction")
run_test("dataUpgradeTest_CParameter"       "CParameter")
run_test("dataUpgradeTest_CAccount"         "CAccount")
run_test("dataUpgradeTest_CAccountWatch"    "CAccountWatch")
run_test("dataUpgradeTest_CBalHistory"      "CBalHistory")
run_test("dataUpgradeTest_CBlock"           "CBlock")
run_test("dataUpgradeTest_CIncomeStatement" "CIncomeStatement")
run_test("dataUpgradeTest_CLogEntry"        "CLogEntry")
run_test("dataUpgradeTest_CPriceQuote"      "CPriceQuote")
run_test("dataUpgradeTest_CReceipt"         "CReceipt")
run_test("dataUpgradeTest_CRPCResult"       "CRPCResult")
run_test("dataUpgradeTest_CTrace"           "CTrace")
run_test("dataUpgradeTest_CTraceAction"     "CTraceAction")
run_test("dataUpgradeTest_CTraceResult"     "CTraceResult")
run_test("dataUpgradeTest_CTransaction"     "CTransaction")
run_test("dataUpgradeTest_CNewBlock"        "CNewBlock")
