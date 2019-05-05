# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# library project
project (util)

# produce the version-git.cpp file with current git branch, hash and timestamp
set (GIT_VERSION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/version_git.cpp)
set (VERSION_BUILD_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/build_scripts/touch-version-file.sh)
add_custom_target(git_version ALL DEPENDS ${GIT_VERSION_FILE})
add_custom_command(OUTPUT ${GIT_VERSION_FILE} COMMAND ${VERSION_BUILD_SCRIPT})
execute_process(COMMAND ${VERSION_BUILD_SCRIPT} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ )
# debugging
#message(STATUS "verion file: " ${GIT_VERSION_FILE})
#message(STATUS "build script: " ${VERSION_BUILD_SCRIPT})
#message(STATUS "current folder: " ${CMAKE_CURRENT_SOURCE_DIR})

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/libs")

# Define the executable to be generated
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

# Installation steps
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)
