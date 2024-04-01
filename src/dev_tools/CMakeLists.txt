cmake_minimum_required (VERSION 3.5)

project (dev_tools)

set(BASE_LIBS util ${CURL_LIBRARIES})
set(BASE_INCLUDE "${CMAKE_SOURCE_DIR}/dev_tools/utillib")

add_subdirectory(utillib)
add_subdirectory(testRunner)
add_subdirectory(goMaker)
