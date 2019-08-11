# set(GOPATH "${CMAKE_CURRENT_BINARY_DIR}/go")
# file(MAKE_DIRECTORY ${GOPATH})

# function(GO_GET TARG)
#     message(MESSAGE " GO_PATH: " ${GOPATH})
#     message(MESSAGE " ARGN: " ${ARGN})
#     add_custom_target(${TARG} env GOPATH=${GOPATH} go get ${ARGN})
# endfunction(GO_GET)

function(ADD_GO_INSTALLABLE_PROGRAM NAME MAIN_SRC)
  get_filename_component(MAIN_SRC_ABS ${MAIN_SRC} ABSOLUTE)
  add_custom_target(${NAME})
  add_custom_command(TARGET ${NAME}
                    COMMAND go build
                    -o "${CMAKE_SOURCE_DIR}/../bin/${NAME}"
                    ${CMAKE_GO_FLAGS} ${MAIN_SRC}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                    DEPENDS ${MAIN_SRC_ABS} ${MAIN_SRC})
  # foreach(DEP ${ARGN})
  #   add_dependencies(${NAME} ${DEP})
  # endforeach()
  add_custom_target(${NAME}_all ALL DEPENDS ${NAME})

  install(PROGRAMS ${CMAKE_SOURCE_DIR}/../bin/${NAME} DESTINATION bin)
endfunction(ADD_GO_INSTALLABLE_PROGRAM)

#"${CMAKE_CURRENT_BINARY_DIR}/../../../bin/${NAME}"
