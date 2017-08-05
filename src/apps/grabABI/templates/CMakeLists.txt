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
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Add the project static libs at linking
set(THE_LIBS "${CMAKE_CURRENT_LIST_DIR}/parselib/libparse[{PROJECT_NAME}].a" wallet token ${BASE_LIBS})
target_link_libraries (${PROJECT_NAME} ${THE_LIBS} ncurses)
