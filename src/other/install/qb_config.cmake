# Deploy basic configuration and directories structure for quickblocks

#---------------------------------------------------------------
# setup some paths
#---------------------------------------------------------------
set(DEST_PATH "$ENV{HOME}/.quickBlocks")
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
file(MAKE_DIRECTORY "${DEST_PATH}/makeClass")
file(MAKE_DIRECTORY "${DEST_PATH}/grabABI")
file(MAKE_DIRECTORY "${DEST_PATH}/chifra")
file(MAKE_DIRECTORY "${DEST_PATH}/abis")
file(MAKE_DIRECTORY "${DEST_PATH}/abis/known-000")
file(MAKE_DIRECTORY "${DEST_PATH}/abis/known-005")
file(MAKE_DIRECTORY "${DEST_PATH}/abis/known-010")
file(MAKE_DIRECTORY "${DEST_PATH}/abis/known-015")
file(MAKE_DIRECTORY "${DEST_PATH}/ipfs-hashes")

#---------------------------------------------------------------
# Create main config file, if it does not already exist
#---------------------------------------------------------------
set(MAIN_CONFIG "${DEST_PATH}/quickBlocks.toml")
if (NOT EXISTS "${MAIN_CONFIG}")
	message(STATUS "Copying configuration files ${MAIN_CONFIG}")
	file(COPY "${SOURCE_PATH}/quickBlocks.toml" DESTINATION "${DEST_PATH}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
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
message(STATUS "Copying prefunds file to ${DEST_PATH}/names")
file(COPY "${SOURCE_PATH}/names/names_prefunds.tab" DESTINATION "${DEST_PATH}/names" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)

#---------------------------------------------------------------
# Copy the custom names file (empty), unless it already exists  (this one is the user's)
#---------------------------------------------------------------
set(CUSTOM_NAMES "${DEST_PATH}/names/names_custom.tab")
if (NOT EXISTS "${CUSTOM_NAMES}")
	message(STATUS "Copying custom names file to ${DEST_PATH}/names")
	file(COPY "${SOURCE_PATH}/names/names_custom.tab" DESTINATION "${DEST_PATH}/names" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

#---------------------------------------------------------------
# Copy the sample entities if it does not already exist
#---------------------------------------------------------------
set(CUSTOM_ENTS "${DEST_PATH}/names/entities.csv")
if (NOT EXISTS "${CUSTOM_ENTS}")
	message(STATUS "Copying sample entities file${DEST_PATH}/names")
	file(COPY "${SOURCE_PATH}/names/entities.csv" DESTINATION "${DEST_PATH}/names" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

#---------------------------------------------------------------
# Copy the mock data, but don't unzip it
#---------------------------------------------------------------
message(STATUS "Copying unzipped mock data to ${DEST_PATH}/")
file(COPY "${SOURCE_PATH}/mocked/mocked.tar.gz" DESTINATION "${DEST_PATH}/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)

#---------------------------------------------------------------
# makeClass content
#---------------------------------------------------------------
message(STATUS "Copying makeClass templates to ${DEST_PATH}/makeClass")
file(GLOB TARGET_FILES "${CMAKE_SOURCE_DIR}/../../../src/dev_tools/makeClass/templates/blank*")
foreach(FILE ${TARGET_FILES} )
	#message(STATUS "  Copied file to ${DEST_PATH}/makeClass")
	file(COPY "${FILE}" DESTINATION "${DEST_PATH}/makeClass")
endforeach( FILE )

#---------------------------------------------------------------
# grabABI content
#---------------------------------------------------------------
message(STATUS "Copying grabABI templates to ${DEST_PATH}/grabABI")
file(GLOB TARGET_FILES "${CMAKE_SOURCE_DIR}/../../../src/tools/grabABI/templates/*")
foreach(FILE ${TARGET_FILES} )
    # message(STATUS "  Copied file to ${DEST_PATH}/grabABI")
    file(COPY "${FILE}" DESTINATION "${DEST_PATH}/grabABI")
endforeach( FILE )

#---------------------------------------------------------------
# chifra content
#---------------------------------------------------------------
message(STATUS "Copying chifra templates to ${DEST_PATH}/chifra")
file(GLOB TARGET_FILES "${CMAKE_SOURCE_DIR}/../../../src/apps/chifra/templates/*")
foreach(FILE ${TARGET_FILES} )
	file(COPY "${FILE}" DESTINATION "${DEST_PATH}/chifra")
endforeach( FILE )

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
set(IPFS_HASHES "${DEST_PATH}/ipfs-hashes/pin-manifest.json")
if (NOT EXISTS "${IPFS_HASHES}")
	message(STATUS "Seeding ipfs hash files to ${DEST_PATH}/ipfs-hashes")
	file(COPY "${SOURCE_PATH}/ipfs-hashes/pin-manifest.json" DESTINATION "${DEST_PATH}/ipfs-hashes" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
	file(COPY "${SOURCE_PATH}/ipfs-hashes/empty.fil" DESTINATION "${DEST_PATH}/ipfs-hashes" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

#---------------------------------------------------------------
# Copy the prices files
#---------------------------------------------------------------
set(PRICE_FILE "${DEST_PATH}/cache/prices/poloniex_USDT_ETH.bin")
if (NOT EXISTS "${PRICE_FILE}")
	message(STATUS "Copying price database to ${DEST_PATH}/cache/prices")
	file(COPY "${SOURCE_PATH}/prices/poloniex_USDT_ETH.bin.gz" DESTINATION "${DEST_PATH}/cache/prices" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

#---------------------------------------------------------------
# Copy the ts files
#---------------------------------------------------------------
set(TS_FILE "${DEST_PATH}/ts.bin")
if (NOT EXISTS "${TS_FILE}")
	message(STATUS "Copying ts file to ${DEST_PATH}")
	file(COPY "${SOURCE_PATH}/prices/ts.bin.gz" DESTINATION "${DEST_PATH}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

#---------------------------------------------------------------
# Clear a few bin files, so they get regenerated for each build
#---------------------------------------------------------------
message(STATUS "Removing file: ${DEST_PATH}/cache/names/names.bin")
file(REMOVE "${DEST_PATH}/cache/names/names.bin")
message(STATUS "Removing file: ${DEST_PATH}/cache/abis/known.bin")
file(REMOVE "${DEST_PATH}/cache/abis/known.bin")
