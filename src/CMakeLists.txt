cmake_minimum_required (VERSION 3.5)

# ----------------------------------------------------------------------------------------
project(trueBlocks)

# ----------------------------------------------------------------------------------------
message(STATUS "======== LOOKING FOR PYTHON ========================")
find_package(Python COMPONENTS Interpreter Development)

# ----------------------------------------------------------------------------------------
message(STATUS "======== LOOKING FOR CURL ========================")
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

# ----------------------------------------------------------------------------------------
set(BIN_DIR ${CMAKE_SOURCE_DIR}/../bin)
set(EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/../bin/examples)

# ----------------------------------------------------------------------------------------
set(CMAKE_INSTALL_PREFIX "/usr/local/" CACHE PATH "Default install directory")

# ----------------------------------------------------------------------------------------
set(REPO_DIR ${CMAKE_SOURCE_DIR}/..)
set(SCRIPTS_DIR ${REPO_DIR}/scripts)

add_custom_target(lint COMMAND "golangci-lint" "run" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/apps/chifra")
add_custom_target(generate COMMAND ${BIN_DIR}/goMaker WORKING_DIRECTORY ${REPO_DIR})
add_custom_target(test-all COMMAND ${SCRIPTS_DIR}/test-all.sh WORKING_DIRECTORY ${REPO_DIR}/build)
add_custom_target(examples ALL COMMAND ${SCRIPTS_DIR}/build-examples.sh WORKING_DIRECTORY ${REPO_DIR})

# ----------------------------------------------------------------------------------------
function(ADD_GO_INSTALLABLE_PROGRAM2 NAME MAIN_SRC DEST_DIR)
    get_filename_component(MAIN_SRC_ABS ${MAIN_SRC} ABSOLUTE)
    add_custom_target(${NAME} ALL)
    add_custom_command(TARGET ${NAME}
                    COMMAND go build
                    -o "${DEST_DIR}/${NAME}"
                    ${CMAKE_GO_FLAGS} ${MAIN_SRC}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                    DEPENDS ${MAIN_SRC_ABS} ${MAIN_SRC})
endfunction(ADD_GO_INSTALLABLE_PROGRAM2)

# ----------------------------------------------------------------------------------------
add_subdirectory(other/install)

ADD_GO_INSTALLABLE_PROGRAM2(chifra ${CMAKE_SOURCE_DIR}/apps/chifra/*.go ${BIN_DIR})
ADD_GO_INSTALLABLE_PROGRAM2(goMaker ${CMAKE_SOURCE_DIR}/dev_tools/goMaker/*.go ${BIN_DIR})
ADD_GO_INSTALLABLE_PROGRAM2(testRunner ${CMAKE_SOURCE_DIR}/dev_tools/testRunner/*.go ${BIN_DIR})
ADD_GO_INSTALLABLE_PROGRAM2(sdkFuzzer ${CMAKE_SOURCE_DIR}/dev_tools/sdkFuzzer/*.go ${BIN_DIR})

add_dependencies(testRunner goMaker)
add_dependencies(sdkFuzzer goMaker)
add_dependencies(chifra testRunner)
add_dependencies(examples chifra)
