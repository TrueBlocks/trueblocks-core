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
    message(STATUS "${CColor}Could not determine Go Version from output: \
        ${GO_VERSION_OUTPUT}.${COff}")
    message(STATUS "${CColor}Please install Go from \
        https://golang.org/doc/install.${COff}")
    return()
endif()

# Go Version -- NOTE THE LESS 25 marker
#
set(GO_MAJOR_VERSION ${CMAKE_MATCH_1})
set(GO_MINOR_VERSION ${CMAKE_MATCH_2})
if(GO_MAJOR_VERSION LESS 1 OR
    (GO_MAJOR_VERSION EQUAL 1 AND GO_MINOR_VERSION LESS 25))
    message(STATUS "${CColor}The build failed.${COff}")
    message(STATUS "${CColor}Found ${GO_VERSION_OUTPUT}. Go Version 1.25.1 or "
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

# ----------------------------------------------------------------------------
find_program(GOLANGCI_LINT golangci-lint)
if(GOLANGCI_LINT)
    # Build lint command dynamically based on what submodules exist
    set(LINT_COMMANDS "")
    list(APPEND LINT_COMMANDS COMMAND ${GOLANGCI_LINT} run --config ${CMAKE_SOURCE_DIR}/../.golangci.yml ${CMAKE_SOURCE_DIR}/../chifra/...)
    
    if(EXISTS "${CMAKE_SOURCE_DIR}/../sdk/go.mod")
        list(APPEND LINT_COMMANDS COMMAND ${GOLANGCI_LINT} run --config ${CMAKE_SOURCE_DIR}/../.golangci.yml ${CMAKE_SOURCE_DIR}/../sdk/...)
    endif()
    
    if(EXISTS "${CMAKE_SOURCE_DIR}/../khedra/go.mod")
        list(APPEND LINT_COMMANDS COMMAND ${GOLANGCI_LINT} run --config ${CMAKE_SOURCE_DIR}/../.golangci.yml ${CMAKE_SOURCE_DIR}/../khedra/...)
    endif()
    
    if(EXISTS "${CMAKE_SOURCE_DIR}/../dev-tools/testRunner/go.mod")
        list(APPEND LINT_COMMANDS COMMAND ${GOLANGCI_LINT} run --config ${CMAKE_SOURCE_DIR}/../.golangci.yml ${CMAKE_SOURCE_DIR}/../dev-tools/testRunner/...)
    endif()
    
    if(EXISTS "${CMAKE_SOURCE_DIR}/../dev-tools/goMaker/go.mod")
        list(APPEND LINT_COMMANDS COMMAND ${GOLANGCI_LINT} run --config ${CMAKE_SOURCE_DIR}/../.golangci.yml ${CMAKE_SOURCE_DIR}/../dev-tools/goMaker/...)
    endif()

    add_custom_target(lint
        ${LINT_COMMANDS}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/..
        COMMENT "Running golangci-lint across all available Go modules in the repo"
    )
else()
    message(WARNING "golangci-lint not found. Please install it to use 'make lint'.")
endif()

# ----------------------------------------------------------------------------
add_custom_target(generate
    COMMAND goMaker
    WORKING_DIRECTORY ${REPO_DIR})

# ----------------------------------------------------------------------------
add_custom_target(test-all
    COMMAND bash -c "TB_NO_KHEDRA_TEST=true ${SCRIPTS_DIR}/test-all.sh"
    WORKING_DIRECTORY ${REPO_DIR}/build)

# ----------------------------------------------------------------------------
# Only build examples if the submodule is present
if(EXISTS "${REPO_DIR}/examples/.git" OR EXISTS "${REPO_DIR}/examples/simple/go.mod")
    add_custom_target(examples ALL
        COMMAND ${SCRIPTS_DIR}/build-examples.sh
        WORKING_DIRECTORY ${REPO_DIR})
else()
    message(STATUS "Examples submodule not found, skipping examples build")
endif()

# ----------------------------------------------------------------------------
# Only build khedra if the submodule is present
if(EXISTS "${REPO_DIR}/khedra/go.mod")
    add_custom_target(khedra ALL
        COMMAND ${SCRIPTS_DIR}/build-khedra.sh
        WORKING_DIRECTORY ${REPO_DIR})
else()
    message(STATUS "Khedra submodule not found, skipping khedra build")
endif()

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
    ${CMAKE_SOURCE_DIR}/../chifra ${BIN_DIR})

# Only build goMaker if the submodule is present
if(EXISTS "${CMAKE_SOURCE_DIR}/../dev-tools/goMaker/go.mod")
    add_go_installable_program(goMaker
        ${CMAKE_SOURCE_DIR}/../dev-tools/goMaker /usr/local/bin)
else()
    message(STATUS "goMaker submodule not found, skipping goMaker build")
endif()

if(NOT WIN32)
    # Only build testRunner if the submodule is present
    if(EXISTS "${CMAKE_SOURCE_DIR}/../dev-tools/testRunner/go.mod")
        add_go_installable_program(testRunner
            ${CMAKE_SOURCE_DIR}/../dev-tools/testRunner ${BIN_DIR})
        
        # Only add goMaker dependency if goMaker exists
        if(EXISTS "${CMAKE_SOURCE_DIR}/../dev-tools/goMaker/go.mod")
            add_dependencies(testRunner goMaker)
        endif()
        
        add_dependencies(chifra testRunner)
    else()
        message(STATUS "testRunner submodule not found, skipping testRunner build")
    endif()
endif()

# Only add dependencies if the submodules exist
if(EXISTS "${REPO_DIR}/examples/.git" OR EXISTS "${REPO_DIR}/examples/simple/go.mod")
    add_dependencies(examples chifra)
endif()

if(EXISTS "${REPO_DIR}/khedra/go.mod")
    add_dependencies(khedra chifra)
endif()
