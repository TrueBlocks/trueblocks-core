# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (makeClass)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "makeClass")
set(PROJ_NAME "dev_tools")
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
#add_custom_target(test-state COMMAND ${CMAKE_CTEST_COMMAND} -C test_state)

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

# Enter one line for each individual test
run_test("makeClass_README"    "-th")
run_test("makeClass_noparams1"      )
run_test("makeClass_noparams2"  "x")
run_test("makeClass_nonexist"   "-e" " x")
run_test("makeClass_conflict1"  "-e" " --run")
run_test("makeClass_conflict2"  "-l" " -r")
run_test("makeClass_conflict3"  "-c" " -r")
run_test("makeClass_conflict4"  "-c" " -r" " -e")
run_test("makeClass_clear"      "-c" " function")
run_test("makeClass_list"       "--list" " function")
run_test("makeClass_edit"       "-e" " function")
run_test("makeClass_run"        "--run" " block")
run_test("makeClass_run_ns"     "-n" " qblocks" " -r" " function")
run_test("makeClass_js"         "--js" " CBlock")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
