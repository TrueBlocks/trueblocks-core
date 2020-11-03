# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# application project
project (turboDive)

# The sources to be used
file(GLOB SOURCE_FILES "*.cpp")

# Output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../bin")

# Define the executable to be generated
set(TOOL_NAME "turboDive")
set(PROJ_NAME "tools")
add_executable(${TOOL_NAME} ${SOURCE_FILES})

# Add the project static libs at linking
set(SILKWORM ${CMAKE_CURRENT_SOURCE_DIR}/libs/libsilkworm.a)
set(LMDB ${CMAKE_CURRENT_SOURCE_DIR}/libs/liblmdb.a)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Werror -Wall -Wno-sign-conversion -O2 -fPIC")
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Werror -Wconversion -Wall -Wno-sign-conversion -O2 -fPIC")
    target_link_libraries (${TOOL_NAME} ${BASE_LIBS} ${SILKWORM} ${LMDB})
else()
    target_link_libraries (${TOOL_NAME} ${BASE_LIBS} ${SILKWORM} ${LMDB} -lpthread)
endif()

include_directories(${BASE_INCLUDE} ${CMAKE_CURRENT_SOURCE_DIR})

# Additional target to make the README.md
build_readme(${CMAKE_CURRENT_SOURCE_DIR} ${TOOL_NAME})

# Installation steps
install(TARGETS ${TOOL_NAME} RUNTIME DESTINATION bin)
