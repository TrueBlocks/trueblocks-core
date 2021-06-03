# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# folder project
project (examples)

# messaging
# message (WARNING  "*** Entering examples folder ***")

# Compilation order of the src subdirectories
add_subdirectory(simple)
add_subdirectory(visitAll)
add_subdirectory(txCounter)
add_subdirectory(findFirst)
add_subdirectory(dynamic)
