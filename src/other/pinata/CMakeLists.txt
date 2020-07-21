# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# application project
project (pinata)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/bin")

# Define the executable to be generated
set(TOOL_NAME "pinata")
add_executable(${TOOL_NAME} ${SOURCE_FILES})

# Additional target to make the README.md
build_readme(${CMAKE_CURRENT_SOURCE_DIR} ${TOOL_NAME})

# Add the project static libs at linking
target_link_libraries (${TOOL_NAME} ${BASE_LIBS})
