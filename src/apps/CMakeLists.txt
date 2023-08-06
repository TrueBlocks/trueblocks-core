cmake_minimum_required (VERSION 3.5)

# folder project
project(apps)

# Path to library include files
include_directories(${BASE_INCLUDE})

# messaging
# message (WARNING  "*** Entering apps folder ***")

# Compilation order of the src subdirectories
add_subdirectory(chifra)
