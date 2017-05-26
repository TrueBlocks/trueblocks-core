# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# project
project (etherlib)

# c++ compilation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O2")

# Headers required, it depends on utillib and abilib
include_directories("${CMAKE_SOURCE_DIR}/libs/utillib")
include_directories("${CMAKE_SOURCE_DIR}/libs/abilib")

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/libs")

# Define the executable to be generated
add_library(etherlib STATIC ${SOURCE_FILES})

# Copy the library headers, make them available for the rest of modules
set(COMMON_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/libs/include")
file(GLOB HEADER_FILES "*.h*")
file(COPY ${HEADER_FILES} DESTINATION ${COMMON_INCLUDE_PATH})

# plint target
set(CURRENT_TOOL_PATH "${CMAKE_SOURCE_DIR}/libs/etherlib")
add_custom_target(${LINT_TARGET}
        COMMAND ${PYLINT_CMD} ${SOURCE_FILES}
        WORKING_DIRECTORY ${CURRENT_TOOL_PATH}
        COMMENT "Running pylint for ${TOOL_NAME}"
        VERBATIM)

