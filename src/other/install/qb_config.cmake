# Deploy basic configuration and directories structure for quickblocks

set(QUICKBLOCKS_HOME "$ENV{HOME}/.quickBlocks")

# create the files (will silently fail if already present)
message(STATUS "Establishing configuration folders at ${QUICKBLOCKS_HOME}")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}/configs")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}/slurps")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}/abis")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}/makeClass")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}/grabABI")
file(MAKE_DIRECTORY "${QUICKBLOCKS_HOME}/chifra")

# Create toml file only if it does not exist
set(QUICKBLOCKS_TOML_FILE "${QUICKBLOCKS_HOME}/quickBlocks.toml")
if (NOT EXISTS "${QUICKBLOCKS_TOML_FILE}")
    message(STATUS "Copying configuration file ${QUICKBLOCKS_TOML_FILE}")
    file(COPY "${CMAKE_SOURCE_DIR}/../../../src/other/install/quickBlocks.toml" DESTINATION "${QUICKBLOCKS_HOME}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endif()

