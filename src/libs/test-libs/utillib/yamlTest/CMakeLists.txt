# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# application project
project (yamlTest)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${BIN_TEST_DIR}")

# Define the executable to be generated
set(TOOL_NAME "yamlTest")
set(PROJ_NAME "libs/utillib")
add_executable(${TOOL_NAME} ${SOURCE_FILES})

# Add the project static libs at linking
target_link_libraries (${TOOL_NAME} ${BASE_LIBS})
