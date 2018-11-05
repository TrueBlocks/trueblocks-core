# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# folder project
project (monitors)

# Path to library include files
include_directories(${BASE_INCLUDE})

# messaging
# message (WARNING  "*** Entering monitor folder ***")

# Compilation order of the src subdirectories

add_subdirectory(scripts)

add_subdirectory(accts/parselib)
add_subdirectory(accts)

add_subdirectory(firstAcct/parselib)
add_subdirectory(firstAcct)

add_subdirectory(rchain/parselib)
add_subdirectory(rchain)

add_subdirectory(augur/parselib)
add_subdirectory(augur)

add_subdirectory(bnt/parselib)
add_subdirectory(bnt)

add_subdirectory(dao/parselib)
add_subdirectory(dao)

add_subdirectory(ENS/parselib)
add_subdirectory(ENS)

#add_subdirectory(etherTip/parselib)
#add_subdirectory(etherTip)

add_subdirectory(firstBlood/parselib)
add_subdirectory(firstBlood)

add_subdirectory(gnosis/parselib)
add_subdirectory(gnosis)

add_subdirectory(paritySMS/parselib)
add_subdirectory(paritySMS)

add_subdirectory(singular/parselib)
add_subdirectory(singular)

add_subdirectory(tenX/parselib)
add_subdirectory(tenX)

add_subdirectory(whiteHat/parselib)
add_subdirectory(whiteHat)

add_subdirectory(tierion/parselib)
add_subdirectory(tierion)

add_subdirectory(fyn/parselib)
add_subdirectory(fyn)

add_subdirectory(maker/parselib)
add_subdirectory(maker)

add_subdirectory(quantstamp/parselib)
add_subdirectory(quantstamp)

add_subdirectory(binance/parselib)
add_subdirectory(binance)

add_subdirectory(giveth_ff/parselib)
add_subdirectory(giveth_ff)

add_subdirectory(expertyHack/parselib)
add_subdirectory(expertyHack)

add_subdirectory(customer_1/parselib)
add_subdirectory(customer_1)

add_subdirectory(uportClaims/parselib)
add_subdirectory(uportClaims)

add_subdirectory(addressHandle/parselib)
add_subdirectory(addressHandle)

add_subdirectory(walton/parselib)
add_subdirectory(walton)

add_subdirectory(forRichard/parselib)
add_subdirectory(forRichard)

add_subdirectory(eSports/parselib)
add_subdirectory(eSports)

add_subdirectory(superRare/parselib)
add_subdirectory(superRare)

add_subdirectory(fomo3d/parselib)
add_subdirectory(fomo3d)

add_subdirectory(aragon/parselib)
add_subdirectory(aragon)

add_subdirectory(chris/parselib)
add_subdirectory(chris)

#add_subdirectory(etherTip/parselib)
#add_subdirectory(etherTip)

#add_subdirectory(gonetwork/parselib)
#add_subdirectory(gonetwork)

#add_subdirectory(newbium/parselib)
#add_subdirectory(newbium)

#add_subdirectory(status/parselib)
#add_subdirectory(status)
