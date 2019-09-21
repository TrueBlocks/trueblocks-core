# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# folder project
project (test-utillib)

# messaging
# message (WARNING  "*** Entering libs folder ***")

# Compile the libraries in this order
add_subdirectory(colorTest)
add_subdirectory(convert)
add_subdirectory(jsonTest)
add_subdirectory(stringTest)
add_subdirectory(tomlTest)
add_subdirectory(tomlTest2)
add_subdirectory(bigIntTest)
add_subdirectory(apiTest)
add_subdirectory(cmdLineTest)
add_subdirectory(typesTest)
add_subdirectory(curlTest)
add_subdirectory(sizeof)
add_subdirectory(arrayTest)
add_subdirectory(parsingTest)
add_subdirectory(fileTest)
