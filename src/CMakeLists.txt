cmake_minimum_required (VERSION 3.5)

# ----------------------------------------------------------------------------------------
# folder project
project(trueBlocks)

# ----------------------------------------------------------------------------------------
# Allow targets in cascade
set_property(GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS 1)

# ----------------------------------------------------------------------------------------
# Mandatory python3 interpret, the directive PythonInterp sets the following variables
message(STATUS "======== LOOKING FOR PYTHON ========================")
find_package(Python COMPONENTS Interpreter Development)

# ----------------------------------------------------------------------------------------
# We require the CURL library to be installed for most apps and tools
# if found add its include path to includes section and link with it
message(STATUS "======== LOOKING FOR CURL ========================")
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

# ----------------------------------------------------------------------------------------
set(BIN_DIR ${CMAKE_SOURCE_DIR}/../bin)
set(EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/../bin/examples)
set(BIN_CMDS_DIR $ENV{HOME}/.local/bin/chifra)
set(BIN_TEST_DIR ${BIN_CMDS_DIR}/test)

# ----------------------------------------------------------------------------------------
set(CMAKE_INSTALL_PREFIX "/usr/local/" CACHE PATH "Default install directory")

# ----------------------------------------------------------------------------------------
function(ADD_GO_INSTALLABLE_PROGRAM NAME MAIN_SRC DEST_DIR)
    get_filename_component(MAIN_SRC_ABS ${MAIN_SRC} ABSOLUTE)
    add_custom_target(${NAME})
    add_custom_command(TARGET ${NAME}
                    COMMAND go build
                    -o "${DEST_DIR}/${NAME}"
                    ${CMAKE_GO_FLAGS} ${MAIN_SRC}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                    DEPENDS ${MAIN_SRC_ABS} ${MAIN_SRC})
    add_custom_target(${NAME}_all ALL DEPENDS ${NAME})
endfunction(ADD_GO_INSTALLABLE_PROGRAM)

# ----------------------------------------------------------------------------------------
# Examples
set(REPO_DIR ${CMAKE_SOURCE_DIR}/..)
set(SCRIPTS_DIR ${REPO_DIR}/scripts)
message(STATUS "REPO_DIR: ${REPO_DIR}")
message(STATUS "SCRIPTS_DIR: ${SCRIPTS_DIR}")
message(STATUS "CMAKE_COMMAND: ${CMAKE_COMMAND}")

# ----------------------------------------------------------------------------------------
# Enable testing
# enable_testing()
add_custom_target(lint COMMAND "golangci-lint" "run" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/apps/chifra")
add_custom_target(generate COMMAND "${BIN_DIR}/goMaker" "" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/..")
add_custom_target(test-all COMMAND "${BIN_TEST_DIR}/test-all.sh" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../build")
add_custom_target(examples
  COMMAND ${CMAKE_COMMAND} -E echo "Building Go examples..."
  COMMAND ${SCRIPTS_DIR}/build_examples.sh
  WORKING_DIRECTORY ${REPO_DIR}
  COMMENT "Go examples have been built."
)

# ----------------------------------------------------------------------------------------
# Compilation order for the subfolders
add_subdirectory(dev_tools)
add_subdirectory(apps)
add_subdirectory(examples)
add_subdirectory(other)
