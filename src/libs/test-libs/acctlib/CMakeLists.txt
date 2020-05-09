# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# folder project
project (test-acctlib)

# messaging
# message (WARNING  "*** Entering libs folder ***")

# Compile the libraries in this order
add_subdirectory(acctTest)
add_subdirectory(ensTest)
