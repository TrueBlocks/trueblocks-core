# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

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
target_link_libraries (${TOOL_NAME} ${BASE_LIBS})

# Additional target to make the README.md
build_readme(${CMAKE_CURRENT_SOURCE_DIR} ${PROJ_NAME} ${TOOL_NAME} "when")

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
