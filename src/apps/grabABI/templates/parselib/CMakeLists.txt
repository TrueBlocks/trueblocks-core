# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# parser project
project (parse[{PROJECT_NAME}])

# c++ compilation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror -Wall -O2")

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

# Define the executable to be generated
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

# Headers required, it depends on utillib and abilib
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libs/utillib")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libs/abilib")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libs/etherlib")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libs/tokenlib")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libs/walletlib")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/..")
