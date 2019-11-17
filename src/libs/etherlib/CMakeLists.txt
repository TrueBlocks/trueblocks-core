# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# library project
project (ether)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/libs")

# Define the executable to be generated
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

# Headers require
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libs/utillib")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libs/abilib")

# Installation steps
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)
