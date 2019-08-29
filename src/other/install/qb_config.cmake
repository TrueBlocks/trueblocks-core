# Deploy basic configuration and directories structure for quickblocks

set(QUICKBLOCKS_HOME "$ENV{HOME}/.quickBlocks")

# create the files (will silently fail if already present)
message(STATUS "Establishing configuration folders at ${QUICKBLOCKS_HOME}")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}/names")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}/cache")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}/cache/abis")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}/cache/prices")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}/cache/tmp")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}/known_abis")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}/makeClass")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}/grabABI")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}/chifra")

# Create toml file only if it does not exist
set(QUICKBLOCKS_TOML_FILE "${QUICKBLOCKS_HOME}/quickBlocks.toml")
if (NOT EXISTS "${QUICKBLOCKS_TOML_FILE}")
    message(STATUS "Copying configuration files ${QUICKBLOCKS_TOML_FILE}")
    file(COPY "${CMAKE_SOURCE_DIR}/../../../src/other/install/quickBlocks.toml" DESTINATION "${QUICKBLOCKS_HOME}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

# Create toml file only if it does not exist
set(QUICKBLOCKS_TOML_FILE "${QUICKBLOCKS_HOME}/ethslurp.toml")
if (NOT EXISTS "${QUICKBLOCKS_TOML_FILE}")
    message(STATUS "Copying custom configuration file ${QUICKBLOCKS_TOML_FILE}")
    file(COPY "${CMAKE_SOURCE_DIR}/../../../src/other/install/ethslurp.toml" DESTINATION "${QUICKBLOCKS_HOME}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

# Copy the names file which replaces the .txt file
set(QUICKBLOCKS_NAMES1_FILE "${QUICKBLOCKS_HOME}/names/names.txt")
message(STATUS "Copying names file ${QUICKBLOCKS_NAMES1_FILE}")
file(COPY "${CMAKE_SOURCE_DIR}/../../../src/other/install/names.txt" DESTINATION "${QUICKBLOCKS_HOME}/names/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)

# Clear the bin file. It will get regenerated.
message(STATUS "Removing binary name file: ${QUICKBLOCKS_HOME}/names/names.bin")
file(REMOVE "${QUICKBLOCKS_HOME}/names/names.bin")

# Copy the custom names file unless it already exists
set(QUICKBLOCKS_NAMES2_FILE "${QUICKBLOCKS_HOME}/names/names_custom.txt")
if (NOT EXISTS "${QUICKBLOCKS_NAMES2_FILE}")
    message(STATUS "Copying names file ${QUICKBLOCKS_NAMES2_FILE}")
    file(COPY "${CMAKE_SOURCE_DIR}/../../../src/other/install/names_custom.txt" DESTINATION "${QUICKBLOCKS_HOME}/names/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

# Copy the prices files
message(STATUS "Copying price database to ${QUICKBLOCKS_HOME}/cache/prices/")
set(QUICKBLOCKS_PRICES_FILE "${QUICKBLOCKS_HOME}/cache/prices/poloniex_USDT_ETH.bin")
if (NOT EXISTS "${QUICKBLOCKS_PRICES_FILE}")
    set(QUICKBLOCKS_PRICES_SOURCE "${CMAKE_SOURCE_DIR}/../../../src/other/install/prices/poloniex_USDT_ETH.bin.gz")
    message(STATUS "  Copied file to ${QUICKBLOCKS_HOME}/cache/prices/")
    file(COPY "${QUICKBLOCKS_PRICES_SOURCE}" DESTINATION "${QUICKBLOCKS_HOME}/cache/prices" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()
set(QUICKBLOCKS_TS_FILE "${QUICKBLOCKS_HOME}/ts.bin")
if (NOT EXISTS "${QUICKBLOCKS_TS_FILE}")
    set(QUICKBLOCKS_TS_SOURCE "${CMAKE_SOURCE_DIR}/../../../src/other/install/prices/ts.bin.gz")
    message(STATUS "  Copied file to ${QUICKBLOCKS_HOME}/")
    file(COPY "${QUICKBLOCKS_TS_SOURCE}" DESTINATION "${QUICKBLOCKS_HOME}/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

# prefund
set(QUICKBLOCKS_PREFUND_FILE "${QUICKBLOCKS_HOME}/prefunds.txt")
if (NOT EXISTS "${QUICKBLOCKS_PREFUND_FILE}")
    message(STATUS "Copying prefund file ${QUICKBLOCKS_PREFUND_FILE}")
    file(COPY "${CMAKE_SOURCE_DIR}/../../../src/other/install/prefunds.txt" DESTINATION "${QUICKBLOCKS_HOME}/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

# makeClass content
message(STATUS "Copying makeClass templates to ${QUICKBLOCKS_HOME}/makeClass/")
file(COPY "${CMAKE_SOURCE_DIR}/../../../bin/makeClass" DESTINATION "${QUICKBLOCKS_HOME}/makeClass")
file(GLOB TARGET_FILES "${CMAKE_SOURCE_DIR}/../../../src/dev_tools/makeClass/templates/blank*")
foreach(FILE ${TARGET_FILES} )
    message(STATUS "  Copied file to ${QUICKBLOCKS_HOME}/makeClass/")
	file(COPY "${FILE}" DESTINATION "${QUICKBLOCKS_HOME}/makeClass/")
endforeach( FILE )

# grabABI content
message(STATUS "Copying grabABI templates to ${QUICKBLOCKS_HOME}/grabABI/")
file(GLOB TARGET_FILES "${CMAKE_SOURCE_DIR}/../../../src/tools/grabABI/templates/*")
foreach(FILE ${TARGET_FILES} )
	file(COPY "${FILE}" DESTINATION "${QUICKBLOCKS_HOME}/grabABI")
endforeach( FILE )

# chifra content
message(STATUS "Copying chifra templates to ${QUICKBLOCKS_HOME}/chifra/")
file(GLOB TARGET_FILES "${CMAKE_SOURCE_DIR}/../../../src/apps/chifra/templates/*")
foreach(FILE ${TARGET_FILES} )
      file(COPY "${FILE}" DESTINATION "${QUICKBLOCKS_HOME}/chifra")
endforeach( FILE )

# known abis
message(STATUS "Copying known abis to ${QUICKBLOCKS_HOME}/known_abis/")
file(GLOB TARGET_FILES "${CMAKE_SOURCE_DIR}/../../../src/other/install/known_abis/*")
foreach(FILE ${TARGET_FILES} )
      file(COPY "${FILE}" DESTINATION "${QUICKBLOCKS_HOME}/known_abis")
endforeach( FILE )
