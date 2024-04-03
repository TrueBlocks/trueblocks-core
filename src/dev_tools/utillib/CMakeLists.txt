cmake_minimum_required (VERSION 3.5)

# library project
project(util)

# produce the version-git.cpp file with current git branch, hash and timestamp

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/dev_tools/")

# Define the executable to be generated
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/dev_tools/utillib")
