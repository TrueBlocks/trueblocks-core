cmake_minimum_required (VERSION 3.5)

function(ADD_GO_INSTALLABLE_PROGRAM NAME MAIN_SRC DEST_DIR)
    get_filename_component(MAIN_SRC_ABS ${MAIN_SRC} ABSOLUTE)
    add_custom_target(${NAME} ALL)
    add_custom_command(TARGET ${NAME}
                    COMMAND go build
                    -o "${DEST_DIR}/${NAME}"
                    ${CMAKE_GO_FLAGS} ${MAIN_SRC}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                    DEPENDS ${MAIN_SRC_ABS} ${MAIN_SRC})
endfunction(ADD_GO_INSTALLABLE_PROGRAM)

add_subdirectory(simple)
add_subdirectory(findFirst)
add_subdirectory(allContracts)
add_subdirectory(usesSDK)
add_subdirectory(balanceChart)
add_subdirectory(optimism1)
