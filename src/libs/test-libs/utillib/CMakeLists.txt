# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# folder project
project (test-utillib)

# messaging
# message (WARNING  "*** Entering libs folder ***")

# Compile the libraries in this order
add_subdirectory(arrayTest)
add_subdirectory(bigIntTest)
add_subdirectory(builtInTest)
add_subdirectory(cmdLineTest)
add_subdirectory(colorTest)
add_subdirectory(convertTest)
add_subdirectory(curlTest)
add_subdirectory(dateTest)
add_subdirectory(fileTest)
add_subdirectory(jsonTest)
add_subdirectory(loggingTest)
add_subdirectory(parsingTest)
add_subdirectory(sizeofTest)
add_subdirectory(stringTest)
add_subdirectory(timeTest)
add_subdirectory(tomlTest)
add_subdirectory(tomlTest2)
add_subdirectory(typesTest)
add_subdirectory(runningTest)
add_subdirectory(versionTest)
add_subdirectory(yamlTest)
