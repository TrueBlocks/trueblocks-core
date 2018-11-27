# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# folder project
project (libs)

# messaging
# message (WARNING  "*** Entering libs folder ***")

# Compile the libraries in this order
add_subdirectory (utillib)
add_subdirectory (etherlib)
add_subdirectory (acctlib)
add_subdirectory (tokenlib)
add_subdirectory (walletlib)
add_subdirectory (test-libs)

# Headers installation steps
file(GLOB HEADER_FILES "utillib/*.h" "etherlib/*.h" "acctlib/*.h" "tokenlib/*.h" "walletlib/*.h")
install (FILES ${HEADER_FILES} DESTINATION include)