# Deploy basic configuration and directories structure for quickblocks

set(QUICKBLOCKS_HOME "$ENV{HOME}/.quickBlocks")

# create the files (will silently fail if already present)
message(STATUS "Establishing configuration folders at ${QUICKBLOCKS_HOME}")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}/cache")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}/cache/abis")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}/cache/names")
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
set(QUICKBLOCKS_TOML_FILE "${QUICKBLOCKS_HOME}/whenBlock.toml")
if (NOT EXISTS "${QUICKBLOCKS_TOML_FILE}")
    message(STATUS "Copying custom configuration file ${QUICKBLOCKS_TOML_FILE}")
    file(COPY "${CMAKE_SOURCE_DIR}/../../../src/other/install/whenBlock.toml" DESTINATION "${QUICKBLOCKS_HOME}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

# Create names fie only if it does not exist
set(QUICKBLOCKS_NAMES_FILE "${QUICKBLOCKS_HOME}/cache/names/names.txt")
if (NOT EXISTS "${QUICKBLOCKS_NAMES_FILE}")
    message(STATUS "Copying names file ${QUICKBLOCKS_NAMES_FILE}")
    file(COPY "${CMAKE_SOURCE_DIR}/../../../src/other/install/names.txt" DESTINATION "${QUICKBLOCKS_HOME}/cache/names" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

# makeClass content
file(COPY "${CMAKE_SOURCE_DIR}/../../../bin/makeClass" DESTINATION "${QUICKBLOCKS_HOME}/makeClass")
file(GLOB TARGET_FILES "${CMAKE_SOURCE_DIR}/../../../src/apps/makeClass/templates/blank*")
foreach(FILE ${TARGET_FILES} ) 
	file(COPY "${FILE}" DESTINATION "${QUICKBLOCKS_HOME}/makeClass")
endforeach( FILE ) 

# grabABI content
file(GLOB TARGET_FILES "${CMAKE_SOURCE_DIR}/../../../src/apps/grabABI/templates/*")
foreach(FILE ${TARGET_FILES} )
	file(COPY "${FILE}" DESTINATION "${QUICKBLOCKS_HOME}/grabABI")
endforeach( FILE )

# chifra content
file(GLOB TARGET_FILES "${CMAKE_SOURCE_DIR}/../../../src/apps/chifra/templates/*")
foreach(FILE ${TARGET_FILES} )
      file(COPY "${FILE}" DESTINATION "${QUICKBLOCKS_HOME}/chifra")
endforeach( FILE )

# cache content
file(GLOB TARGET_FILES "${CMAKE_SOURCE_DIR}/../../../src/apps/grabABI/known_contracts/*")
foreach(FILE ${TARGET_FILES} )
      file(COPY "${FILE}" DESTINATION "${QUICKBLOCKS_HOME}/cache/abis")
endforeach( FILE )
