# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# library project
project (util)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/libs")

# Define the executable to be generated
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

# Installation steps
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)
