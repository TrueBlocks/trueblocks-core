# ---------------------------------------------------------------
if(NOT WIN32)
	string(ASCII 27 Esc)
	set(COff "${Esc}[m")

	# set(CColor  "${Esc}[1;37m") bright white
	set(CColor "${Esc}[1;35m") # magenta
endif()

# print a message
function(PrintLine MSG1)
	# message(STATUS ${MSG1})
endfunction(PrintLine)

# copy the file but only if its not present
function(CopyNotPresent FROM_DIR IN_FILE TO_DIR)
	if(NOT EXISTS "${TO_DIR}/${IN_FILE}")
		PrintLine("   ${CColor}Copying ./${IN_FILE} to ${TO_DIR}${COff}")
		file(COPY "${FROM_DIR}/${IN_FILE}" DESTINATION "${TO_DIR}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
	else()
		PrintLine("   ${CColor}Not copying ./${IN_FILE}, already present${COff}")
	endif()
endfunction(CopyNotPresent)

# copy the file even if it's present (that is, replace it)
function(CopyIgnorePresent FROM_DIR IN_FILE TO_DIR)
	PrintLine("   ${CColor}Copying ./${IN_FILE} to ${TO_DIR}${COff}")
	file(COPY "${FROM_DIR}/${IN_FILE}" DESTINATION "${TO_DIR}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endfunction(CopyIgnorePresent)

# copy and touch the file even if it's present (that is, replace it)
function(CopyAndTouchIgnorePresent FROM_DIR IN_FILE TO_DIR)
	PrintLine("   ${CColor}Copying ./${IN_FILE} to ${TO_DIR}${COff}")
	file(TOUCH "${FROM_DIR}/${IN_FILE}")
	file(COPY "${FROM_DIR}/${IN_FILE}" DESTINATION "${TO_DIR}" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
endfunction(CopyAndTouchIgnorePresent)

# copy an entire folder, replacing even if existing
function(CopyFolder FROM_DIR TO_DIR)
	STRING(REGEX REPLACE ${INSTALL_SOURCE} "." FROM_STR ${FROM_DIR})
	PrintLine("   ${CColor}Copying ${FROM_STR}* to ${TO_DIR}${COff}")
	file(GLOB TARGET_FILES "${FROM_DIR}/*")

	foreach(FILE ${TARGET_FILES})
		file(COPY "${FILE}" DESTINATION "${TO_DIR}/" FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
	endforeach(FILE)
endfunction(CopyFolder)

# ---------------------------------------------------------------
set(INSTALL_SOURCE "${CMAKE_CURRENT_LIST_DIR}")

# ---------------------------------------------------------------
set(INSTALL_DEST "$ENV{XDG_CONFIG_HOME}")

if("${INSTALL_DEST}" STREQUAL "")
	if(WIN32)
		PrintLine("Windows build is not supported")
	elseif(APPLE)
		set(INSTALL_DEST "$ENV{HOME}/Library/Application Support/TrueBlocks")
	else()
		set(INSTALL_DEST "$ENV{HOME}/.local/share/trueblocks")
	endif()
endif()

# ---------------------------------------------------------------
PrintLine("Building...")
PrintLine("   ${CColor}from: ${INSTALL_SOURCE}${COff}")
PrintLine("   ${CColor}to:   ${INSTALL_DEST}${COff}")

# ---------------------------------------------------------------
PrintLine("Creating folders...")
PrintLine("   ${CColor}folders: abis, cache, config, unchained${COff}")

file(MAKE_DIRECTORY "${INSTALL_DEST}")
file(MAKE_DIRECTORY "${INSTALL_DEST}/cache")
file(MAKE_DIRECTORY "${INSTALL_DEST}/cache/mainnet")
file(MAKE_DIRECTORY "${INSTALL_DEST}/unchained")
file(MAKE_DIRECTORY "${INSTALL_DEST}/unchained/mainnet/")

file(MAKE_DIRECTORY "${INSTALL_DEST}/abis")
file(MAKE_DIRECTORY "${INSTALL_DEST}/abis/known-000")
file(MAKE_DIRECTORY "${INSTALL_DEST}/abis/known-005")
file(MAKE_DIRECTORY "${INSTALL_DEST}/abis/known-010")
file(MAKE_DIRECTORY "${INSTALL_DEST}/abis/known-015")
file(MAKE_DIRECTORY "${INSTALL_DEST}/perf")

# ---------------------------------------------------------------
PrintLine("Copying files and folder to config...")

# ---------------------------------------------------------------
# Function        InFolder                                 InFile                     OutFolder
CopyNotPresent    (${INSTALL_SOURCE}                   "trueBlocks.toml"          ${INSTALL_DEST})

CopyIgnorePresent (${INSTALL_SOURCE}/per-chain/mainnet "allocs.csv"               ${INSTALL_DEST}/config/mainnet/)
CopyIgnorePresent (${INSTALL_SOURCE}/per-chain/gnosis  "allocs.csv"               ${INSTALL_DEST}/config/gnosis/)
CopyIgnorePresent (${INSTALL_SOURCE}/per-chain/sepolia "allocs.csv"               ${INSTALL_DEST}/config/sepolia/)
CopyIgnorePresent (${INSTALL_SOURCE}/per-chain/goerli  "allocs.csv"               ${INSTALL_DEST}/config/goerli/)
CopyIgnorePresent (${INSTALL_SOURCE}/per-chain/polygon "allocs.csv"               ${INSTALL_DEST}/config/polygon/)

CopyAndTouchIgnorePresent (${INSTALL_SOURCE}/names/    "names.tab"                ${INSTALL_DEST}/config/mainnet/)
CopyAndTouchIgnorePresent (${INSTALL_SOURCE}/names/    "names.tab"                ${INSTALL_DEST}/config/gnosis/)
CopyAndTouchIgnorePresent (${INSTALL_SOURCE}/names/    "names.tab"                ${INSTALL_DEST}/config/sepolia/)

CopyIgnorePresent (${INSTALL_SOURCE}/per-chain/mainnet "specials.csv"             ${INSTALL_DEST}/config/mainnet/)
CopyIgnorePresent (${INSTALL_SOURCE}/per-chain/gnosis  "specials.csv"             ${INSTALL_DEST}/config/gnosis/)
CopyIgnorePresent (${INSTALL_SOURCE}/per-chain/sepolia "specials.csv"             ${INSTALL_DEST}/config/sepolia/)

CopyNotPresent    (${INSTALL_SOURCE}/per-chain/mainnet "manifest.json"            ${INSTALL_DEST}/config/mainnet/)
CopyNotPresent    (${INSTALL_SOURCE}/per-chain/gnosis  "manifest.json"            ${INSTALL_DEST}/config/gnosis/)
CopyNotPresent    (${INSTALL_SOURCE}/per-chain/sepolia "manifest.json"            ${INSTALL_DEST}/config/sepolia/)

CopyNotPresent    (${INSTALL_SOURCE}/                  "ethslurp.toml"            ${INSTALL_DEST}/config/mainnet/)
CopyNotPresent    (${INSTALL_SOURCE}/names/            "names_custom.tab"         ${INSTALL_DEST}/config/mainnet/)

CopyFolder        (${INSTALL_SOURCE}/abis/known-000/                              ${INSTALL_DEST}/abis/known-000/)
CopyFolder        (${INSTALL_SOURCE}/abis/known-005/                              ${INSTALL_DEST}/abis/known-005/)
CopyFolder        (${INSTALL_SOURCE}/abis/known-010/                              ${INSTALL_DEST}/abis/known-010/)
CopyFolder        (${INSTALL_SOURCE}/abis/known-015/                              ${INSTALL_DEST}/abis/known-015/)

# ---------------------------------------------------------------
PrintLine("Removing files...")
PrintLine("   ${CColor}${INSTALL_DEST}/cache/names/*.bin${COff}")
file(REMOVE "${INSTALL_DEST}/cache/names/allocs.bin")
file(REMOVE "${INSTALL_DEST}/cache/names/names.bin")
file(REMOVE "${INSTALL_DEST}/cache/names/names_prefunds_bals.bin")
file(REMOVE "${INSTALL_DEST}/cache/allocs.bin") # weird
PrintLine("   ${CColor}${INSTALL_DEST}/cache/abis/known.bin${COff}")
file(REMOVE "${INSTALL_DEST}/cache/abis/known.bin")

PrintLine("Cleaning old files...")
file(REMOVE "${INSTALL_DEST}/config/mainnet/manifest.txt")
file(REMOVE "${INSTALL_DEST}/abis/known-000/openzephlin.json")
