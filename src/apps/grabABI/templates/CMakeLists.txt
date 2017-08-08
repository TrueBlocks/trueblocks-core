# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# monitor project
project ([{PROJECT_NAME}])

# local headers for parser and debugger
include_directories("${CMAKE_CURRENT_LIST_DIR}/parselib" "${CMAKE_CURRENT_LIST_DIR}")

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/bin")

# Define the executable to be generated
set(TOOL_NAME "[{PROJECT_NAME}]")
add_executable(${TOOL_NAME} ${SOURCE_FILES})

# Add the project static libs at linking
set(THE_LIBS "${CMAKE_CURRENT_LIST_DIR}/parselib/libparse[{PROJECT_NAME}].a" wallet token ${BASE_LIBS})
target_link_libraries (${PROJECT_NAME} ${THE_LIBS} ncurses)

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
