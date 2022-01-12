# Deploy basic configuration and directories structure for trueblocks

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
message(STATUS "TB_INSTALL_FOLDER set to [" ${TB_INSTALL_FOLDER} "]")
set(DEST_PATH ${TB_INSTALL_FOLDER})
set(SOURCE_PATH "${CMAKE_SOURCE_DIR}/../../../src/other/install")

#---------------------------------------------------------------
# create needed folders (will silently fail if already present)
#---------------------------------------------------------------
message(STATUS "Establishing configuration folders at ${DEST_PATH}")
file(MAKE_DIRECTORY "${DEST_PATH}")
file(MAKE_DIRECTORY "${DEST_PATH}/cache")
file(MAKE_DIRECTORY "${DEST_PATH}/cache/abis")
file(MAKE_DIRECTORY "${DEST_PATH}/cache/prices")
file(MAKE_DIRECTORY "${DEST_PATH}/cache/tmp")
file(MAKE_DIRECTORY "${DEST_PATH}/names")
file(MAKE_DIRECTORY "${DEST_PATH}/abis")
file(MAKE_DIRECTORY "${DEST_PATH}/abis/known-000")
file(MAKE_DIRECTORY "${DEST_PATH}/abis/known-005")
file(MAKE_DIRECTORY "${DEST_PATH}/abis/known-010")
file(MAKE_DIRECTORY "${DEST_PATH}/abis/known-015")
file(MAKE_DIRECTORY "${DEST_PATH}/manifest")
file(MAKE_DIRECTORY "${DEST_PATH}/mocked")

#---------------------------------------------------------------
# Create main config file, if it does not already exist
#---------------------------------------------------------------
set(MAIN_CONFIG "${DEST_PATH}/trueBlocks.toml")
if (NOT EXISTS "${MAIN_CONFIG}")
	message(STATUS "Copying configuration files ${MAIN_CONFIG}")
	file(COPY "${SOURCE_PATH}/trueBlocks.toml" DESTINATION "${DEST_PATH}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

#---------------------------------------------------------------
# Create Etherslurp configuration file, if it does not already exist
#---------------------------------------------------------------
set(ETHSLURP_CONFIG "${DEST_PATH}/ethslurp.toml")
if (NOT EXISTS "${ETHSLURP_CONFIG}")
	message(STATUS "Copying custom configuration file ${ETHSLURP_CONFIG}")
	file(COPY "${SOURCE_PATH}/ethslurp.toml" DESTINATION "${DEST_PATH}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

#---------------------------------------------------------------
# Always copy the names.txt file (this one is ours, so we can overwrite)
#---------------------------------------------------------------
message(STATUS "Copying names file to ${DEST_PATH}/names")
file(COPY "${SOURCE_PATH}/names/names.tab" DESTINATION "${DEST_PATH}/names" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)

#---------------------------------------------------------------
# Always copy the verified.txt file (this one is ours, so we can overwrite)
#---------------------------------------------------------------
message(STATUS "Copying verified file to ${DEST_PATH}/names")
file(COPY "${SOURCE_PATH}/names/verified.tab" DESTINATION "${DEST_PATH}/names" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)

#---------------------------------------------------------------
# Copy the prefunds even if it already exists (this one is ours - it never changes)
#---------------------------------------------------------------
message(STATUS "Copying allocations file to ${DEST_PATH}/config")
file(COPY "${SOURCE_PATH}/allocs/fermion/allocs.csv" DESTINATION "${DEST_PATH}/config/fermion/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${SOURCE_PATH}/allocs/gnosis/allocs.csv" DESTINATION "${DEST_PATH}/config/gnosis/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${SOURCE_PATH}/allocs/goerli/allocs.csv" DESTINATION "${DEST_PATH}/config/goerli/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${SOURCE_PATH}/allocs/kovan/allocs.csv" DESTINATION "${DEST_PATH}/config/kovan/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${SOURCE_PATH}/allocs/mainnet/allocs.csv" DESTINATION "${DEST_PATH}/config/mainnet/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${SOURCE_PATH}/allocs/rinkeby/allocs.csv" DESTINATION "${DEST_PATH}/config/rinkeby/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${SOURCE_PATH}/allocs/ropsten/allocs.csv" DESTINATION "${DEST_PATH}/config/ropsten/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)

#---------------------------------------------------------------
# Copy the custom names file (empty), unless it already exists  (this one is the user's)
#---------------------------------------------------------------
set(CUSTOM_NAMES "${DEST_PATH}/names/names_custom.tab")
if (NOT EXISTS "${CUSTOM_NAMES}")
	message(STATUS "Copying custom names file to ${DEST_PATH}/names")
	file(COPY "${SOURCE_PATH}/names/names_custom.tab" DESTINATION "${DEST_PATH}/names" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

#---------------------------------------------------------------
# Copy the sample collections if it does not already exist
#---------------------------------------------------------------
set(CUSTOM_ENTS "${DEST_PATH}/names/collections.csv")
if (NOT EXISTS "${CUSTOM_ENTS}")
	message(STATUS "Copying sample collections file${DEST_PATH}/names")
	file(COPY "${SOURCE_PATH}/names/collections.csv" DESTINATION "${DEST_PATH}/names" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

#---------------------------------------------------------------
# Copy the mock data and testing monitors
#---------------------------------------------------------------
message(STATUS "Copying mocked data to ${DEST_PATH}/mocked")
file(COPY "${SOURCE_PATH}/mocked/mocks.tar.gz" DESTINATION "${DEST_PATH}/mocked/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
file(COPY "${SOURCE_PATH}/mocked/monitors.tar.gz" DESTINATION "${DEST_PATH}/mocked/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)

#---------------------------------------------------------------
# abis
#---------------------------------------------------------------
message(STATUS "Copying abis to ${DEST_PATH}/abis/")
file(GLOB TARGET_FILES "${SOURCE_PATH}/abis/known-000/*")
foreach(FILE ${TARGET_FILES} )
	file(COPY "${FILE}" DESTINATION "${DEST_PATH}/abis/known-000/")
endforeach( FILE )
file(GLOB TARGET_FILES "${SOURCE_PATH}/abis/known-005/*.json")
foreach(FILE ${TARGET_FILES} )
	file(COPY "${FILE}" DESTINATION "${DEST_PATH}/abis/known-005/")
endforeach( FILE )
file(GLOB TARGET_FILES "${SOURCE_PATH}/abis/known-010/*.json")
foreach(FILE ${TARGET_FILES} )
	file(COPY "${FILE}" DESTINATION "${DEST_PATH}/abis/known-010/")
endforeach( FILE )
file(GLOB TARGET_FILES "${SOURCE_PATH}/abis/known-015/*.json")
foreach(FILE ${TARGET_FILES} )
	file(COPY "${FILE}" DESTINATION "${DEST_PATH}/abis/known-015/")
endforeach( FILE )

#---------------------------------------------------------------
# Copy the ipfs hash files (if they don't exist -- user may be building them)
#---------------------------------------------------------------
set(MANIFEST "${DEST_PATH}/manifest/manifest.txt")
if (NOT EXISTS "${MANIFEST}")
	message(STATUS "Seeding initial manifest ${DEST_PATH}/manifest/")
	file(COPY "${SOURCE_PATH}/manifest/manifest.txt" DESTINATION "${DEST_PATH}/manifest/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

#---------------------------------------------------------------
# Copy the prices files
#---------------------------------------------------------------
message(STATUS "Copying price database to ${DEST_PATH}")
file(COPY "${SOURCE_PATH}/prices/poloniex_USDT_ETH.bin.gz" DESTINATION "${DEST_PATH}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)

#---------------------------------------------------------------
# Copy the ts files
#---------------------------------------------------------------
message(STATUS "Copying ts.bin.gz file to ${DEST_PATH}")
file(COPY "${SOURCE_PATH}/prices/ts.bin.gz" DESTINATION "${DEST_PATH}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)

#---------------------------------------------------------------
# Clear a few bin files, so they get regenerated for each build
#---------------------------------------------------------------
message(STATUS "Removing file: ${DEST_PATH}/cache/names/names.bin")
file(REMOVE "${DEST_PATH}/cache/names/names.bin")
file(REMOVE "${DEST_PATH}/cache/names/names_prefunds_bals.bin")
file(REMOVE "${DEST_PATH}/cache/allocs.bin")

message(STATUS "Removing file: ${DEST_PATH}/cache/abis/known.bin")
file(REMOVE "${DEST_PATH}/cache/abis/known.bin")
