# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# monitor project
project ([{PROJECT_NAME}])

# local headers for parser and debugger
include_directories("${CMAKE_CURRENT_LIST_DIR}/parselib" "${CMAKE_CURRENT_LIST_DIR}")

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../monitors/${PROJECT_NAME}/bin")

# Define the executable to be generated
set(TOOL_NAME "[{PROJECT_NAME}]")
set(PROJ_NAME "monitors")
add_executable(${TOOL_NAME} ${SOURCE_FILES})

# Add the project static libs at linking
set(THE_LIBS "${CMAKE_CURRENT_LIST_DIR}/parselib/libparse[{PROJECT_NAME}].a" ${BASE_LIBS})
target_link_libraries (${PROJECT_NAME} ${THE_LIBS} ncurses)
