# Deploy basic configuration and directories structure for trueblocks.

#---------------------------------------------------------------
# setup some paths
#---------------------------------------------------------------
set(TB_INSTALL_FOLDER "$ENV{XDG_CONFIG_HOME}")
if(WIN32)
    message(STATUS "Windows build is not supported yet")
elseif(APPLE)
    if ("${TB_INSTALL_FOLDER}" STREQUAL "")
        set(TB_INSTALL_FOLDER "$ENV{HOME}/Library/Application Support/TrueBlocks")
    endif()
else()
    if ("${TB_INSTALL_FOLDER}" STREQUAL "")
        set(TB_INSTALL_FOLDER "$ENV{HOME}/.local/share/trueblocks")
    endif()
endif()
message(STATUS "Installing to [" ${TB_INSTALL_FOLDER} "]")
set(ROOT_CONFIG ${TB_INSTALL_FOLDER})
set(INSTALL_SOURCE "${CMAKE_SOURCE_DIR}/../../../src/other/install")

#---------------------------------------------------------------
# create needed folders (will silently fail if already present)
#---------------------------------------------------------------
message(STATUS "Establishing configuration folders at ${ROOT_CONFIG}")
file(MAKE_DIRECTORY "${ROOT_CONFIG}")
file(MAKE_DIRECTORY "${ROOT_CONFIG}/cache")
file(MAKE_DIRECTORY "${ROOT_CONFIG}/cache/abis")
file(MAKE_DIRECTORY "${ROOT_CONFIG}/cache/prices")
file(MAKE_DIRECTORY "${ROOT_CONFIG}/cache/tmp")
file(MAKE_DIRECTORY "${ROOT_CONFIG}/cache/objs")
file(MAKE_DIRECTORY "${ROOT_CONFIG}/abis")
file(MAKE_DIRECTORY "${ROOT_CONFIG}/abis/known-000")
file(MAKE_DIRECTORY "${ROOT_CONFIG}/abis/known-005")
file(MAKE_DIRECTORY "${ROOT_CONFIG}/abis/known-010")
file(MAKE_DIRECTORY "${ROOT_CONFIG}/abis/known-015")
file(MAKE_DIRECTORY "${ROOT_CONFIG}/manifest")
file(MAKE_DIRECTORY "${ROOT_CONFIG}/mocked")
file(MAKE_DIRECTORY "${ROOT_CONFIG}/unchained")

#---------------------------------------------------------------
# Create main config file, if it does not already exist
#---------------------------------------------------------------
set(MAIN_CONFIG "${ROOT_CONFIG}/trueBlocks.toml")
if (NOT EXISTS "${MAIN_CONFIG}")
	message(STATUS "Copying configuration files ${MAIN_CONFIG}")
	file(COPY "${INSTALL_SOURCE}/trueBlocks.toml" DESTINATION "${ROOT_CONFIG}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

#---------------------------------------------------------------
# Create Etherslurp configuration file, if it does not already exist
#---------------------------------------------------------------
set(ETHSLURP_CONFIG "${ROOT_CONFIG}/ethslurp.toml")
if (NOT EXISTS "${ETHSLURP_CONFIG}")
	message(STATUS "Copying custom configuration file ${ETHSLURP_CONFIG}")
	file(COPY "${INSTALL_SOURCE}/ethslurp.toml" DESTINATION "${ROOT_CONFIG}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

#---------------------------------------------------------------
# Copy the prefunds even if it already exists (this one is ours - it never changes)
#---------------------------------------------------------------
message(STATUS "Copying allocations file to ${ROOT_CONFIG}/config")
file(COPY "${INSTALL_SOURCE}/allocs/fermion/allocs.csv" DESTINATION "${ROOT_CONFIG}/config/fermion/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${INSTALL_SOURCE}/allocs/gnosis/allocs.csv" DESTINATION "${ROOT_CONFIG}/config/gnosis/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${INSTALL_SOURCE}/allocs/goerli/allocs.csv" DESTINATION "${ROOT_CONFIG}/config/goerli/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${INSTALL_SOURCE}/allocs/kovan/allocs.csv" DESTINATION "${ROOT_CONFIG}/config/kovan/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${INSTALL_SOURCE}/allocs/mainnet/allocs.csv" DESTINATION "${ROOT_CONFIG}/config/mainnet/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${INSTALL_SOURCE}/allocs/rinkeby/allocs.csv" DESTINATION "${ROOT_CONFIG}/config/rinkeby/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${INSTALL_SOURCE}/allocs/ropsten/allocs.csv" DESTINATION "${ROOT_CONFIG}/config/ropsten/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)

#---------------------------------------------------------------
# Always copy the names.txt file (this one is ours, so we can overwrite)
#---------------------------------------------------------------
message(STATUS "Copying names file to ${ROOT_CONFIG}/config/mainnet/")
file(COPY "${INSTALL_SOURCE}/names/names.tab" DESTINATION "${ROOT_CONFIG}/config/mainnet/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)

#---------------------------------------------------------------
# Copy the custom names file (empty), unless it already exists  (this one is the user's)
#---------------------------------------------------------------
set(CUSTOM_NAMES "${ROOT_CONFIG}/config/mainnet/names_custom.tab")
if (NOT EXISTS "${CUSTOM_NAMES}")
	message(STATUS "Copying custom names file to ${ROOT_CONFIG}/config/mainnet/")
	file(COPY "${INSTALL_SOURCE}/names/names_custom.tab" DESTINATION "${ROOT_CONFIG}/config/mainnet/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

#---------------------------------------------------------------
# Copy the sample collections if it does not already exist
#---------------------------------------------------------------
set(CUSTOM_ENTS "${ROOT_CONFIG}/config/mainnet/collections.csv")
if (NOT EXISTS "${CUSTOM_ENTS}")
	message(STATUS "Copying sample collections file${ROOT_CONFIG}/config/mainnet/")
	file(COPY "${INSTALL_SOURCE}/names/collections.csv" DESTINATION "${ROOT_CONFIG}/config/mainnet/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

#---------------------------------------------------------------
# Always copy the verified.txt file (this one is ours, so we can overwrite)
#---------------------------------------------------------------
message(STATUS "Copying verified file to ${ROOT_CONFIG}/config/mainnet/")
file(COPY "${INSTALL_SOURCE}/names/verified.tab" DESTINATION "${ROOT_CONFIG}/config/mainnet/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)

#---------------------------------------------------------------
# Copy the ipfs hash files (if they don't exist -- user may be building them)
#---------------------------------------------------------------
set(MANIFEST "${ROOT_CONFIG}/manifest/manifest.txt")
if (NOT EXISTS "${MANIFEST}")
	message(STATUS "Seeding initial manifest ${ROOT_CONFIG}/manifest/")
	file(COPY "${INSTALL_SOURCE}/manifest/manifest.txt" DESTINATION "${ROOT_CONFIG}/manifest/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

#---------------------------------------------------------------
# Copy the mock data and testing monitors
#---------------------------------------------------------------
message(STATUS "Copying mocked data to ${ROOT_CONFIG}/mocked")
file(COPY "${INSTALL_SOURCE}/mocked/mocks.tar.gz" DESTINATION "${ROOT_CONFIG}/mocked/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${INSTALL_SOURCE}/mocked/monitors.tar.gz" DESTINATION "${ROOT_CONFIG}/mocked/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)

#---------------------------------------------------------------
# abis
#---------------------------------------------------------------
message(STATUS "Copying abis to ${ROOT_CONFIG}/abis/")
file(GLOB TARGET_FILES "${INSTALL_SOURCE}/abis/known-000/*")
foreach(FILE ${TARGET_FILES} )
	file(COPY "${FILE}" DESTINATION "${ROOT_CONFIG}/abis/known-000/")
endforeach( FILE )
file(GLOB TARGET_FILES "${INSTALL_SOURCE}/abis/known-005/*.json")
foreach(FILE ${TARGET_FILES} )
	file(COPY "${FILE}" DESTINATION "${ROOT_CONFIG}/abis/known-005/")
endforeach( FILE )
file(GLOB TARGET_FILES "${INSTALL_SOURCE}/abis/known-010/*.json")
foreach(FILE ${TARGET_FILES} )
	file(COPY "${FILE}" DESTINATION "${ROOT_CONFIG}/abis/known-010/")
endforeach( FILE )
file(GLOB TARGET_FILES "${INSTALL_SOURCE}/abis/known-015/*.json")
foreach(FILE ${TARGET_FILES} )
	file(COPY "${FILE}" DESTINATION "${ROOT_CONFIG}/abis/known-015/")
endforeach( FILE )

#---------------------------------------------------------------
# Copy the prices files
#---------------------------------------------------------------
message(STATUS "Copying price database to ${ROOT_CONFIG}")
file(COPY "${INSTALL_SOURCE}/prices/poloniex_USDT_ETH.bin.gz" DESTINATION "${ROOT_CONFIG}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)

#---------------------------------------------------------------
# Copy the ts files
#---------------------------------------------------------------
message(STATUS "Copying ts.bin.gz file to ${ROOT_CONFIG}")
file(COPY "${INSTALL_SOURCE}/prices/ts.bin.gz" DESTINATION "${ROOT_CONFIG}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)

#---------------------------------------------------------------
# Clear a few bin files, so they get regenerated for each build
#---------------------------------------------------------------
message(STATUS "Removing file: ${ROOT_CONFIG}/cache/names/names.bin")
file(REMOVE "${ROOT_CONFIG}/cache/names/names.bin")
file(REMOVE "${ROOT_CONFIG}/cache/names/names_prefunds_bals.bin")
file(REMOVE "${ROOT_CONFIG}/cache/allocs.bin")

message(STATUS "Removing file: ${ROOT_CONFIG}/cache/abis/known.bin")
file(REMOVE "${ROOT_CONFIG}/cache/abis/known.bin")
