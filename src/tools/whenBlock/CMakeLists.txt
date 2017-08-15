# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (whenBlock)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "whenBlock")
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
     run_the_test(${TEST_PATH} ${GOLD_PATH} ${testName} ${TEST_EXE} ${ARGN})
endfunction(run_test)

# Function to Concatenate multiple files in a single output file
# Rename or move to root CMakeLists.txt if you cosider it handy
#
# Parameters : 
# input_list - input files (list)
# output_file - output file
function(concatenate_files input_list output_file)
    file(WRITE ${OUT} "")
    foreach(FILENAME ${input_list})
        file(READ ${FILENAME} CONTENTS)
	file(APPEND ${output_file} "${CONTENTS}")
    endforeach()
endfunction(concatenate_files)

# We need to generate for test 01 the input files blocks.txt and dates.txt. How?

# NOTE FROM JAY: I PUT THESE FILES IN THE GOLD FOLDER AND CHANGE CWD TO GOLD TO RUN THE TEST

# Group them in a list
#set(INPUT_LIST "/path/to/blocks.txt" "/path/To/dates.txt")

# Once we have them we can concatenate both files using the function concatenate_files
# set(OUTPUT_TEST_FILE "/path/to/results.txt")
#add_custom_command(OUTPUT ${OUTPUT_TEST_FILE} COMMAND concatenate_files ${INPUT_LIST} ${OUTPUT_TEST_FILE})


# Enter one line for each individual test
run_test("whenBlock_README" "-th")
run_test("whenBlock_noparams"    )
#broken -- HOW CAN I CAT THE CONTENTS OF THE FILES INTO THE TEST RESULT?
#run_test("whenBlock_01" ";cat blocks.txt dates.txt")
# Run now the test that depends on the custom_command so it is run first
#run_test("whenBlock_01" "${OUTPUT_TEST_FILE}")
run_test("whenBlock_02" "--file:blocks.txt")
run_test("whenBlock_03" "--file:dates.txt")
