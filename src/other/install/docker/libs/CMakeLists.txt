# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# folder project
project (libs)

# messaging
# message (WARNING  "*** Entering libs folder ***")

# Compile the libraries in this order
add_subdirectory(utillib)
add_subdirectory(etherlib)
add_subdirectory(acctlib)

# Headers installation steps
file(GLOB HEADER_FILES "utillib/*.h" "etherlib/*.h" "acctlib/*.h")
install (FILES ${HEADER_FILES} DESTINATION include)
