cmake_minimum_required(VERSION 3.16)
if(POLICY CMP0175)
    cmake_policy(SET CMP0175 NEW)
endif()

# ----------------------------------------------------------------------------
if(NOT WIN32)
        string(ASCII 27 Esc)
        set(COff "${Esc}[m")
        set(CColor "${Esc}[1;35m") # magenta
endif()

# ----------------------------------------------------------------------------
project(trueBlocks)

# ----------------------------------------------------------------------------
message(STATUS "======== LOOKING FOR PYTHON ========================")
find_package(Python COMPONENTS Interpreter Development)

# ----------------------------------------------------------------------------
message(STATUS "======== LOOKING FOR CURL ========================")
find_package(CURL REQUIRED)

# ----------------------------------------------------------------------------
message(STATUS "======== LOOKING FOR GOLANG ========================")
find_program(GO_EXECUTABLE NAMES go)
if(NOT GO_EXECUTABLE)
    message(STATUS "${CColor}GoLang not found!${COff}")
    message(STATUS "${CColor}Please install Go from \
        https://golang.org/doc/install.${COff}")
    return()
endif()
execute_process(
    COMMAND ${GO_EXECUTABLE} version
    OUTPUT_VARIABLE GO_VERSION_OUTPUT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REGEX MATCH "go([0-9]+)\\.([0-9]+)\\.[0-9]+"
    GO_VERSION_MATCH "${GO_VERSION_OUTPUT}")

if(NOT GO_VERSION_MATCH)
    message(STATUS "${CColor}Could not determine GoLang version from output: \
        ${GO_VERSION_OUTPUT}.${COff}")
    message(STATUS "${CColor}Please install Go from \
        https://golang.org/doc/install.${COff}")
    return()
endif()

set(GO_MAJOR_VERSION ${CMAKE_MATCH_1})
set(GO_MINOR_VERSION ${CMAKE_MATCH_2})
if(GO_MAJOR_VERSION LESS 1 OR
    (GO_MAJOR_VERSION EQUAL 1 AND GO_MINOR_VERSION LESS 23))
    message(STATUS "${CColor}The build failed.${COff}")
    message(STATUS "${CColor}Found ${GO_VERSION_OUTPUT}. Go Version 1.23 or "
                    "higher is required.${COff}")
    message(STATUS "${CColor}Please install or update Go from "
                    "https://golang.org/doc/install.${COff}")
    return()
else()
    message(STATUS "Found GoLang: ${GO_VERSION_OUTPUT}")
endif()

# ----------------------------------------------------------------------------
set(BIN_DIR ${CMAKE_SOURCE_DIR}/../bin)
set(EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/../bin/examples)

# ----------------------------------------------------------------------------
set(CMAKE_INSTALL_PREFIX "/usr/local/" CACHE PATH "Default install directory")

# ----------------------------------------------------------------------------
set(REPO_DIR ${CMAKE_SOURCE_DIR}/..)
set(SCRIPTS_DIR ${REPO_DIR}/scripts)

add_custom_target(lint
    COMMAND "golangci-lint" "run"
            "--out-format=colored-line-number"
            "--path-prefix=../src/apps/chifra"
            "--timeout=5m" "./..."
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/apps/chifra"
    COMMAND "golangci-lint" "run"
            "--out-format=colored-line-number"
            "--path-prefix=../khedra"
            "--timeout=5m" "./..."
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../khedra"
)

add_custom_target(generate
    COMMAND ${BIN_DIR}/goMaker
    WORKING_DIRECTORY ${REPO_DIR})

add_custom_target(test-all
    COMMAND ${SCRIPTS_DIR}/test-all.sh
    WORKING_DIRECTORY ${REPO_DIR}/build)

add_custom_target(examples ALL
    COMMAND ${SCRIPTS_DIR}/build-examples.sh
    WORKING_DIRECTORY ${REPO_DIR})

add_custom_target(khedra ALL
    COMMAND ${SCRIPTS_DIR}/build-khedra.sh
    WORKING_DIRECTORY ${REPO_DIR})

# ----------------------------------------------------------------------------
function(add_go_installable_program NAME MAIN_SRC DEST_DIR)
    get_filename_component(MAIN_SRC_ABS ${MAIN_SRC} ABSOLUTE)
    file(GLOB SRC_FILE_LIST "${MAIN_SRC}/*.go")
    set(EXE_NAME ${NAME})
    if(WIN32)
        set(EXE_NAME ${NAME}.exe)
    endif()

    add_custom_target(${NAME} ALL)
    add_custom_command(TARGET ${NAME}
                    POST_BUILD
                    COMMAND go build
                    -o "${DEST_DIR}/${EXE_NAME}"
                    ${CMAKE_GO_FLAGS} ${SRC_FILE_LIST}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
endfunction()

# ----------------------------------------------------------------------------
add_subdirectory(other/install)

add_go_installable_program(chifra
    ${CMAKE_SOURCE_DIR}/apps/chifra ${BIN_DIR})
add_go_installable_program(goMaker
    ${CMAKE_SOURCE_DIR}/dev_tools/goMaker ${BIN_DIR})
add_go_installable_program(indexManager
    ${CMAKE_SOURCE_DIR}/dev_tools/indexManager ${BIN_DIR})

if(NOT WIN32)
    add_go_installable_program(testRunner
        ${CMAKE_SOURCE_DIR}/dev_tools/testRunner ${BIN_DIR})
    add_go_installable_program(sdkFuzzer
        ${CMAKE_SOURCE_DIR}/dev_tools/sdkFuzzer ${BIN_DIR})
    add_dependencies(testRunner goMaker)
    add_dependencies(sdkFuzzer goMaker)
    add_dependencies(indexManager goMaker)
    add_dependencies(chifra testRunner)
endif()

add_dependencies(examples chifra)

add_dependencies(khedra chifra)

file(GLOB CHIFRA_FOR_DOCKER_FILES
    "${CMAKE_SOURCE_DIR}/apps/chifra/*.go")
add_custom_target(chifra-for-docker ALL
    COMMAND ${GO_EXECUTABLE} build -o ${BIN_DIR}/chifra
            ${CHIFRA_FOR_DOCKER_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/apps/chifra)
