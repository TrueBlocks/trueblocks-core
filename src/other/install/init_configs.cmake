#---------------------------------------------------------------
function(PrintLine MSG1)
  message(STATUS ${MSG1})
endfunction(PrintLine)
function(CopyNotPresent FROM_DIR IN_FILE TO_DIR)
	if (NOT EXISTS "${TO_DIR}/${IN_FILE}")
		PrintLine("   Copying ./${IN_FILE} to ${TO_DIR}")
		file(COPY "${FROM_DIR}/${IN_FILE}" DESTINATION "${TO_DIR}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
	else()
		PrintLine("   Not copying ./${IN_FILE}, already present")
	endif()
endfunction(CopyNotPresent)
function(CopyIgnorePresent FROM_DIR IN_FILE TO_DIR)
	PrintLine("   Copying ./${IN_FILE} to ${TO_DIR}")
	file(COPY "${FROM_DIR}/${IN_FILE}" DESTINATION "${TO_DIR}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endfunction(CopyIgnorePresent)
function(CopyFolder FROM_DIR TO_DIR)
	STRING(REGEX REPLACE ${INSTALL_SOURCE} "." FROM_STR ${FROM_DIR})
	PrintLine("   Copying ${FROM_STR}* to ${TO_DIR}")
	file(GLOB TARGET_FILES "${FROM_DIR}/*")
	foreach(FILE ${TARGET_FILES} )
		file(COPY "${FILE}" DESTINATION "${TO_DIR}/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
	endforeach(FILE)
endfunction(CopyFolder)

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
set(CONFIG_ROOT "${INSTALL_DEST}/config")
set(ABI_SOURCE "${INSTALL_SOURCE}/abis")
set(ABI_CONFIG "${INSTALL_DEST}/abis")
set(ALLOC_SOURCE "${INSTALL_SOURCE}/allocs")

#---------------------------------------------------------------
PrintLine("Building...")
PrintLine("   from: ${INSTALL_SOURCE}")
PrintLine("   to:   ${INSTALL_DEST}")

#---------------------------------------------------------------
PrintLine("Creating folders...")
PrintLine("   folders: abis, cache, config, unchained")

file(MAKE_DIRECTORY "${INSTALL_DEST}")
file(MAKE_DIRECTORY "${INSTALL_DEST}/cache")
file(MAKE_DIRECTORY "${INSTALL_DEST}/cache/mainnet")
file(MAKE_DIRECTORY "${INSTALL_DEST}/unchained")

file(MAKE_DIRECTORY "${ABI_CONFIG}")
file(MAKE_DIRECTORY "${ABI_CONFIG}/known-000")
file(MAKE_DIRECTORY "${ABI_CONFIG}/known-005")
file(MAKE_DIRECTORY "${ABI_CONFIG}/known-010")
file(MAKE_DIRECTORY "${ABI_CONFIG}/known-015")
file(MAKE_DIRECTORY "${INSTALL_DEST}/manifest")
file(MAKE_DIRECTORY "${INSTALL_DEST}/mocked")
file(MAKE_DIRECTORY "${INSTALL_DEST}/perf")

#---------------------------------------------------------------
PrintLine("Copying files and folder to config...")

#---------------------------------------------------------------
# Function        InFolder                     InFile                     OutFolder
CopyNotPresent    (${INSTALL_SOURCE}           "trueBlocks.toml"          ${INSTALL_DEST}           )
CopyNotPresent    (${INSTALL_SOURCE}/names/    "names_custom.tab"         ${CONFIG_ROOT}/mainnet/   )
CopyNotPresent    (${INSTALL_SOURCE}/names/    "collections.csv"          ${CONFIG_ROOT}/mainnet/   )
CopyNotPresent    (${INSTALL_SOURCE}           "ethslurp.toml"            ${INSTALL_DEST}           )
CopyNotPresent    (${INSTALL_SOURCE}/manifest/ "manifest.txt"             ${INSTALL_DEST}/manifest/ )
CopyIgnorePresent (${INSTALL_SOURCE}/names/    "names.tab"                ${CONFIG_ROOT}/mainnet/   )
CopyIgnorePresent (${INSTALL_SOURCE}/mocked/   "mocks.tar.gz"             ${INSTALL_DEST}/mocked/   )
CopyIgnorePresent (${INSTALL_SOURCE}/mocked/   "monitors.tar.gz"          ${INSTALL_DEST}/mocked/   )
CopyIgnorePresent (${INSTALL_SOURCE}/prices/   "poloniex_USDT_ETH.bin.gz" ${INSTALL_DEST}           )
CopyIgnorePresent (${INSTALL_SOURCE}/prices/   "ts.bin.gz"                ${INSTALL_DEST}           )
CopyIgnorePresent (${ALLOC_SOURCE}/fermion     "allocs.csv"               ${CONFIG_ROOT}/fermion/   )
CopyIgnorePresent (${ALLOC_SOURCE}/gnosis      "allocs.csv"               ${CONFIG_ROOT}/gnosis/    )
CopyIgnorePresent (${ALLOC_SOURCE}/goerli      "allocs.csv"               ${CONFIG_ROOT}/goerli/    )
CopyIgnorePresent (${ALLOC_SOURCE}/kovan       "allocs.csv"               ${CONFIG_ROOT}/kovan/     )
CopyIgnorePresent (${ALLOC_SOURCE}/mainnet     "allocs.csv"               ${CONFIG_ROOT}/mainnet/   )
CopyIgnorePresent (${ALLOC_SOURCE}/rinkeby     "allocs.csv"               ${CONFIG_ROOT}/rinkeby/   )
CopyIgnorePresent (${ALLOC_SOURCE}/ropsten     "allocs.csv"               ${CONFIG_ROOT}/ropsten/   )
CopyFolder        (${ABI_SOURCE}/known-000/                               ${ABI_CONFIG}/known-000/  )
CopyFolder        (${ABI_SOURCE}/known-005/                               ${ABI_CONFIG}/known-005/  )
CopyFolder        (${ABI_SOURCE}/known-010/                               ${ABI_CONFIG}/known-010/  )
CopyFolder        (${ABI_SOURCE}/known-015/                               ${ABI_CONFIG}/known-015/  )

#---------------------------------------------------------------
message(STATUS "Removing files...")
message(STATUS "   ${INSTALL_DEST}/cache/names/names.bin")
file(REMOVE "${INSTALL_DEST}/cache/names/names.bin")
file(REMOVE "${INSTALL_DEST}/cache/names/names_prefunds_bals.bin")
file(REMOVE "${INSTALL_DEST}/cache/allocs.bin")

message(STATUS "   ${INSTALL_DEST}/cache/abis/known.bin")
file(REMOVE "${INSTALL_DEST}/cache/abis/known.bin")
