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
add_subdirectory(pinlib)
add_subdirectory(cachelib)
add_subdirectory(sqlite3lib)
add_subdirectory(test-libs)


install (FILES ${HEADER_FILES} DESTINATION include)
