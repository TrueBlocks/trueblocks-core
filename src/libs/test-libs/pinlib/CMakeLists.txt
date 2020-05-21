# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# folder project
project (test-pinlib)

# messaging
# message (WARNING  "*** Entering libs folder ***")

# Compile the libraries in this order
add_subdirectory(pinTest)
