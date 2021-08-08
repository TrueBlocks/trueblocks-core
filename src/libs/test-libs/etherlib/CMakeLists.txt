# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# folder project
project (test-etherlib)

# messaging
# message (WARNING  "*** Entering libs folder ***")

# Compile the libraries in this order
add_subdirectory(abiTest)
add_subdirectory(abiTest2)
add_subdirectory(binaryPathTest)
add_subdirectory(blockTest)
add_subdirectory(dataUpgradeTest)
add_subdirectory(decodeAbiTest)
add_subdirectory(displayStrsTest)
add_subdirectory(forEveryTest)
add_subdirectory(generalTest)
add_subdirectory(lastBlockTest)
add_subdirectory(printFloatTest)
add_subdirectory(reconTest)
add_subdirectory(serializeTest)
add_subdirectory(sha3Test)
add_subdirectory(traceTest)
