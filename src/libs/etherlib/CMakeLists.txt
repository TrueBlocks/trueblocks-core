# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# library project
project (ether)

# c++ compilation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O2")

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Copy the library headers, make them available for the rest of modules
set(COMMON_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/libs/include")
file(GLOB HEADER_FILES "*.h*")
file(COPY ${HEADER_FILES} DESTINATION ${COMMON_INCLUDE_PATH})

# Output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/libs")

# Define the executable to be generated
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

# Headers required, it depends on utillib and abilib
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libs/utillib")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libs/abilib")
