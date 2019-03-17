# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# folder project
project (test-etherlib)

# messaging
# message (WARNING  "*** Entering libs folder ***")

# Compile the libraries in this order
add_subdirectory(sha3)
add_subdirectory(printFloat)
add_subdirectory(dataUpgrade)
add_subdirectory(serialize)
add_subdirectory(blockTest)
add_subdirectory(forEvery)
add_subdirectory(abiTest)
add_subdirectory(displayStrs)
add_subdirectory(generalTest)
add_subdirectory(decode_abi)
add_subdirectory(lastBlockTest)
