# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# library project
project (util)

# produce the version-git.cpp file with current git branch, hash and timestamp
execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE COMMIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND git log -1 --format=%ct
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE COMMIT_TS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
set_property(
    SOURCE version_git.cpp
    PROPERTY COMPILE_DEFINITIONS
        D_COMMIT_BRANCH=\"${COMMIT_BRANCH}\"
        D_COMMIT_HASH=\"${COMMIT_HASH}\"
        D_COMMIT_TS=${COMMIT_TS}
)
add_custom_target(git_version ALL DEPENDS version_git.cpp)
message (STATUS "GIT: branch '${COMMIT_BRANCH}' commit '${COMMIT_HASH}' timestamp '${COMMIT_TS}'")

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/libs")

# Define the executable to be generated
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

# Headers require
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libs/sqlite3lib")
