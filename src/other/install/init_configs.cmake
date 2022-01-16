#---------------------------------------------------------------
function(PrintLine MSG1)
  message(STATUS ${MSG1})
endfunction(PrintLine)

#---------------------------------------------------------------
set(INSTALL_SOURCE "${CMAKE_CURRENT_LIST_DIR}")

#---------------------------------------------------------------
set(INSTALL_DEST "$ENV{XDG_CONFIG_HOME}")
if ("${INSTALL_DEST}" STREQUAL "")
	if(WIN32)
    	PrintLine("Windows build is not supported")
	elseif(APPLE)
        set(INSTALL_DEST "$ENV{HOME}/Library/Application Support/TrueBlocks")
	else()
        set(INSTALL_DEST "$ENV{HOME}/.local/share/trueblocks")
	endif()
endif()

#---------------------------------------------------------------
PrintLine("Building...")
PrintLine("   from: ${INSTALL_SOURCE}")
PrintLine("   to:   ${INSTALL_DEST}")

#---------------------------------------------------------------
PrintLine("Creating folders...")
PrintLine("   folders: abis, cache, config, unchained")

file(MAKE_DIRECTORY "${INSTALL_DEST}")
file(MAKE_DIRECTORY "${INSTALL_DEST}/cache")
file(MAKE_DIRECTORY "${INSTALL_DEST}/unchained")

file(MAKE_DIRECTORY "${INSTALL_DEST}/abis")
file(MAKE_DIRECTORY "${INSTALL_DEST}/abis/known-000")
file(MAKE_DIRECTORY "${INSTALL_DEST}/abis/known-005")
file(MAKE_DIRECTORY "${INSTALL_DEST}/abis/known-010")
file(MAKE_DIRECTORY "${INSTALL_DEST}/abis/known-015")
file(MAKE_DIRECTORY "${INSTALL_DEST}/manifest")
file(MAKE_DIRECTORY "${INSTALL_DEST}/mocked")
file(MAKE_DIRECTORY "${INSTALL_DEST}/perf")

#---------------------------------------------------------------
PrintLine("Copying files and folder to config...")

#---------------------------------------------------------------
function(CopyNotPresent FROM_DIR FILE TO_DIR)
	if (NOT EXISTS "${TO_DIR}/${FILE}")
		PrintLine("   Copying ./${FILE} to ${TO_DIR}")
		file(COPY "${FROM_DIR}/${FILE}" DESTINATION "${TO_DIR}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
	else()
		PrintLine("   Not copying ./${FILE}, already present")
	endif()
endfunction(CopyNotPresent)

function(CopyIgnorePresent FROM_DIR FILE TO_DIR)
	PrintLine("   Copying ./${FILE} to ${TO_DIR}")
	file(COPY "${FROM_DIR}/${FILE}" DESTINATION "${TO_DIR}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endfunction(CopyIgnorePresent)

set(CHAIN_CONFIG "${INSTALL_DEST}/config")

#---------------------------------------------------------------
CopyNotPresent    (${INSTALL_SOURCE}           "trueBlocks.toml"  ${INSTALL_DEST})
CopyNotPresent    (${INSTALL_SOURCE}           "ethslurp.toml"    ${INSTALL_DEST})
CopyNotPresent    (${INSTALL_SOURCE}/names/    "names_custom.tab" ${CHAIN_CONFIG}/mainnet/)
CopyNotPresent    (${INSTALL_SOURCE}/names/    "collections.csv"  ${CHAIN_CONFIG}/mainnet/)
CopyIgnorePresent (${INSTALL_SOURCE}/names/    "names.tab"        ${CHAIN_CONFIG}/mainnet/)
CopyNotPresent    (${INSTALL_SOURCE}/manifest/ "manifest.txt"     ${INSTALL_DEST}/manifest/)
CopyIgnorePresent (${INSTALL_SOURCE}/mocked/   "mocks.tar.gz"     ${INSTALL_DEST}/mocked/)
CopyIgnorePresent (${INSTALL_SOURCE}/mocked/   "monitors.tar.gz"  ${INSTALL_DEST}/mocked/)

#---------------------------------------------------------------
PrintLine("   allocations file to ${CHAIN_CONFIG}")
file(COPY "${INSTALL_SOURCE}/allocs/fermion/allocs.csv" DESTINATION "${CHAIN_CONFIG}/fermion/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${INSTALL_SOURCE}/allocs/gnosis/allocs.csv" DESTINATION "${CHAIN_CONFIG}/gnosis/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${INSTALL_SOURCE}/allocs/goerli/allocs.csv" DESTINATION "${CHAIN_CONFIG}/goerli/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${INSTALL_SOURCE}/allocs/kovan/allocs.csv" DESTINATION "${CHAIN_CONFIG}/kovan/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${INSTALL_SOURCE}/allocs/mainnet/allocs.csv" DESTINATION "${CHAIN_CONFIG}/mainnet/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${INSTALL_SOURCE}/allocs/rinkeby/allocs.csv" DESTINATION "${CHAIN_CONFIG}/rinkeby/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${INSTALL_SOURCE}/allocs/ropsten/allocs.csv" DESTINATION "${CHAIN_CONFIG}/ropsten/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)

#---------------------------------------------------------------
message(STATUS "   abis to ${INSTALL_DEST}/abis/")
file(GLOB TARGET_FILES "${INSTALL_SOURCE}/abis/known-000/*")
foreach(FILE ${TARGET_FILES} )
	file(COPY "${FILE}" DESTINATION "${INSTALL_DEST}/abis/known-000/")
endforeach( FILE )
file(GLOB TARGET_FILES "${INSTALL_SOURCE}/abis/known-005/*.json")
foreach(FILE ${TARGET_FILES} )
	file(COPY "${FILE}" DESTINATION "${INSTALL_DEST}/abis/known-005/")
endforeach( FILE )
file(GLOB TARGET_FILES "${INSTALL_SOURCE}/abis/known-010/*.json")
foreach(FILE ${TARGET_FILES} )
	file(COPY "${FILE}" DESTINATION "${INSTALL_DEST}/abis/known-010/")
endforeach( FILE )
file(GLOB TARGET_FILES "${INSTALL_SOURCE}/abis/known-015/*.json")
foreach(FILE ${TARGET_FILES} )
	file(COPY "${FILE}" DESTINATION "${INSTALL_DEST}/abis/known-015/")
endforeach( FILE )

#---------------------------------------------------------------
message(STATUS "   price database to ${INSTALL_DEST}")
file(COPY "${INSTALL_SOURCE}/prices/poloniex_USDT_ETH.bin.gz" DESTINATION "${INSTALL_DEST}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)

#---------------------------------------------------------------
# Copy the ts files
#---------------------------------------------------------------
message(STATUS "   ts.bin.gz file to ${INSTALL_DEST}")
file(COPY "${INSTALL_SOURCE}/prices/ts.bin.gz" DESTINATION "${INSTALL_DEST}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)

#---------------------------------------------------------------
# Clear a few bin files, so they get regenerated for each build
#---------------------------------------------------------------
message(STATUS "Removing files...")
message(STATUS "   ${INSTALL_DEST}/cache/names/names.bin")
file(REMOVE "${INSTALL_DEST}/cache/names/names.bin")
file(REMOVE "${INSTALL_DEST}/cache/names/names_prefunds_bals.bin")
file(REMOVE "${INSTALL_DEST}/cache/allocs.bin")

message(STATUS "   ${INSTALL_DEST}/cache/abis/known.bin")
file(REMOVE "${INSTALL_DEST}/cache/abis/known.bin")
