# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

project(libs)

# Compile the libraries in this order
add_subdirectory(utillib)
add_subdirectory(abilib)
add_subdirectory(etherlib)
add_subdirectory(tokenlib)
add_subdirectory(walletlib)

# Debug msg
# message(WARNING  "Building libs")

