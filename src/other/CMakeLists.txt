# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# folder project
project (other)

# messaging
# message (WARNING  "*** Entering other folder ***")

# Compilation order of the src subdirectories
add_subdirectory(install)
add_subdirectory(build_assets)
