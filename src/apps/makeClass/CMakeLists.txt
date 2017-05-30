# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# project
project (makeClass)

# c++ compilation flags used everywhere
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O2 -stdlib=libc++")

# Libraries shared by all the modules
set(PROJECT_INCLUDES "${PROJECT_LIBS_PATH}/include")
include_directories(${PROJECT_INCLUDES})

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
add_executable(makeClass ${SOURCE_FILES})

# We will require the CURL library installed 
# If found add its include path to includes section and link with it
find_package (CURL REQUIRED)
if (CURL_FOUND)
        include_directories(${CURL_INCLUDE_DIRS})
        target_link_libraries (makeClass ${CURL_LIBRARIES})
endif (CURL_FOUND)

# Add the project static libs at linking
target_link_libraries (makeClass abi ether token util wallet)
