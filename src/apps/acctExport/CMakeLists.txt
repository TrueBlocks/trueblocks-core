# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (acctExport)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "acctExport")
set(PROJ_NAME "apps")
add_executable(${TOOL_NAME} ${SOURCE_FILES})

# Add the project static libs at linking
target_link_libraries (${TOOL_NAME} ${BASE_LIBS})

# Testing
# Define paths to test folder and gold folder
set(TEST_EXE "${EXECUTABLE_OUTPUT_PATH}/${TOOL_NAME}")
set(TEST_PATH "${TEST_PATH}/${PROJ_NAME}/${TOOL_NAME}")
set(GOLD_PATH "${GOLD_PATH}/${PROJ_NAME}/${TOOL_NAME}")

# Additional target to make the README.md
build_readme(${CMAKE_CURRENT_SOURCE_DIR} ${TOOL_NAME})

# Target to run only specific (long-running) test cases
add_custom_target(test-all COMMAND ${CMAKE_CTEST_COMMAND} -C test_all)
add_custom_target(test-state COMMAND ${CMAKE_CTEST_COMMAND} -C test_state)

# Functions to run test cases
function(run_test testName)
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_test)
function(run_special_test testName)
    run_the_special_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_special_test)
function(run_state_test testName)
    run_the_state_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_state_test)
function(api_test testName)
    run_the_api_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(api_test)
function(api_special_test testName)
    run_the_api_special_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(api_special_test)
function(api_state_test testName)
    run_the_api_state_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(api_state_test)

# Preload some data
run_the_api_test(${TEST_PATH} ${GOLD_PATH} "acctExport_chifra_rm" ${EXECUTABLE_OUTPUT_PATH}/chifra " rm" " --yes" " 0x001d14804b399c6ef80e64576f657660804fec0b")
run_the_test(${TEST_PATH} ${GOLD_PATH} "acctExport_preload_list" ${EXECUTABLE_OUTPUT_PATH}/acctScrape " 0x001d14804b399c6ef80e64576f657660804fec0b")

# Enter one line for each individual test
run_test("acctExport_no_params")
run_test("acctExport_help"              "-h")

run_test("acctExport_export"            "0x001d14804b399c6ef80e64576f657660804fec0b")
run_test("acctExport_export_apps"       "0x001d14804b399c6ef80e64576f657660804fec0b" " --appearances")
run_test("acctExport_export_traces"     "0x001d14804b399c6ef80e64576f657660804fec0b" " --traces")
run_test("acctExport_export_logs"       "0x001d14804b399c6ef80e64576f657660804fec0b" " --logs")
run_test("acctExport_export_abis"       "0x001d14804b399c6ef80e64576f657660804fec0b" " --abis")

api_test("acctExport_api_export"        "0x001d14804b399c6ef80e64576f657660804fec0b")
api_test("acctExport_api_export_apps"   "0x001d14804b399c6ef80e64576f657660804fec0b" " --appearances")
api_test("acctExport_api_export_traces" "0x001d14804b399c6ef80e64576f657660804fec0b" " --traces")
api_test("acctExport_api_export_logs"   "0x001d14804b399c6ef80e64576f657660804fec0b" " --logs")
api_test("acctExport_api_export_abis"   "0x001d14804b399c6ef80e64576f657660804fec0b" " --abis")

run_state_test("acctExport_export_balances"       "0x001d14804b399c6ef80e64576f657660804fec0b" " --balances")
api_state_test("acctExport_api_export_balances"   "0x001d14804b399c6ef80e64576f657660804fec0b" " --balances")
run_state_test("acctExport_export_balances_d"     "0x001d14804b399c6ef80e64576f657660804fec0b" " --balances" " --deltas")
api_state_test("acctExport_api_export_balances_d" "0x001d14804b399c6ef80e64576f657660804fec0b" " --balances" " --deltas")

run_test("acctExport_export_c"                    "0x001d14804b399c6ef80e64576f657660804fec0b" " --count_only")
run_test("acctExport_export_apps_c"               "0x001d14804b399c6ef80e64576f657660804fec0b" " --appearances" " --count_only")

# Installation steps, this tool is renamed at installation
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
