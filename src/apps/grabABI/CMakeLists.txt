# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# project
project (grabABI)

# c++ compilation flags used everywhere
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O2")

# Libraries shared by all the modules
set(PROJECT_LIBS_PATH "${CMAKE_SOURCE_DIR}/libs")
set(PROJECT_INCLUDES "${PROJECT_LIBS_PATH}/include")
include_directories(${PROJECT_INCLUDES})

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "grabABI")
add_executable(${TOOL_NAME} ${SOURCE_FILES})

# We will require the CURL library installed
# If found add its include path to includes section and link with it
find_package (CURL REQUIRED)
if (CURL_FOUND)
        include_directories(${CURL_INCLUDE_DIRS})
        target_link_libraries (${TOOL_NAME} ${CURL_LIBRARIES})
endif (CURL_FOUND)

# Add the project static libs at linking
target_link_libraries (${TOOL_NAME} util abi ether token wallet curl)
