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
#add_subdirectory(sqlite3lib)
add_subdirectory(test-libs)

file(GLOB A_HEADER_FILES "acctlib/*.h")
install (FILES ${A_HEADER_FILES} DESTINATION include/tb/acctlib)

file(GLOB C_HEADER_FILES "cachelib/*.h")
install (FILES ${C_HEADER_FILES} DESTINATION include/tb/cachelib)

file(GLOB E_HEADER_FILES "etherlib/*.h")
install (FILES ${E_HEADER_FILES} DESTINATION include/tb/etherlib)

file(GLOB P_HEADER_FILES "pinlib/*.h")
install (FILES ${P_HEADER_FILES} DESTINATION include/tb/pinlib)

file(GLOB U_HEADER_FILES "utillib/*.h")
install (FILES ${U_HEADER_FILES} DESTINATION include/tb/utillib)

file(GLOB LIB_FILES "*.a")
install (FILES ${LIB_FILES} DESTINATION lib/tb)

