# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# application project
project (getTokenBal)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "getTokenBal")
add_executable(${TOOL_NAME} ${SOURCE_FILES})

# Add the project static libs at linking
target_link_libraries (${TOOL_NAME} wallet token ${BASE_LIBS})

# Additional target to make the README.md
#set(README_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
#add_custom_target(readme COMMAND ${TEST_EXE} "-th" ">" "${README_FILE}" "2>&1")
