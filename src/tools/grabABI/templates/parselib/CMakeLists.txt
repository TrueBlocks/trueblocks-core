# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# parser project
project (parse[{PROJECT_NAME}])

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

# Define the executable to be generated
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

# Headers require
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libs/utillib")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libs/etherlib")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libs/acctlib")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libs/pinlib")
# target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libs/sqllite3lib")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/..")
