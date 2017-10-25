# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# folder project
project (monitors)

# Path to library include files
include_directories(${BASE_INCLUDE})

# messaging
# message (WARNING  "*** Entering monitor folder ***")

# Compilation order of the src subdirectories
add_subdirectory(cacheMan)

if(0)
add_subdirectory(scripts)
add_subdirectory(bloomMan)

add_subdirectory(accts/parselib)
add_subdirectory(augur/parselib)
add_subdirectory(bnt/parselib)
add_subdirectory(dao/parselib)
add_subdirectory(ENS/parselib)
add_subdirectory(etherTip/parselib)
add_subdirectory(firstBlood/parselib)
add_subdirectory(gnosis/parselib)
add_subdirectory(paritySMS/parselib)
add_subdirectory(singular/parselib)
add_subdirectory(tenX/parselib)
add_subdirectory(whiteHat/parselib)
add_subdirectory(mothership/parselib)

add_subdirectory(accts)
add_subdirectory(augur)
add_subdirectory(bnt)
add_subdirectory(dao)
add_subdirectory(ENS)
add_subdirectory(etherTip)
add_subdirectory(firstBlood)
add_subdirectory(gnosis)
add_subdirectory(paritySMS)
add_subdirectory(singular)
add_subdirectory(tenX)
add_subdirectory(whiteHat)
add_subdirectory(mothership)
endif(0)
