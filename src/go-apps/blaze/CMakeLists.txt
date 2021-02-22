# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# application project
project (blaze)

function(ADD_GO_INSTALLABLE_PROGRAM NAME MAIN_SRC)
  get_filename_component(MAIN_SRC_ABS ${MAIN_SRC} ABSOLUTE)
  add_custom_target(${NAME})
  add_custom_command(TARGET ${NAME}
                    COMMAND go build
                    -o "${CMAKE_SOURCE_DIR}/../bin/${NAME}"
                    ${CMAKE_GO_FLAGS} ${MAIN_SRC}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                    DEPENDS ${MAIN_SRC_ABS} ${MAIN_SRC})
  add_custom_target(${NAME}_all ALL DEPENDS ${NAME})
  install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${NAME} DESTINATION bin)
endfunction(ADD_GO_INSTALLABLE_PROGRAM)

ADD_GO_INSTALLABLE_PROGRAM(blaze *.go)

# Define the executable to be generated
set(TOOL_NAME "blaze")
set(PROJ_NAME "go-apps")

# Testing
# Define paths to test folder and gold folder
set(TEST_EXE "${EXECUTABLE_OUTPUT_PATH}/${TOOL_NAME}")
set(TEST_PATH "${TEST_PATH}/${PROJ_NAME}/${TOOL_NAME}")
set(GOLD_PATH "${GOLD_PATH}/${PROJ_NAME}/${TOOL_NAME}")

# Additional target to make the README.md
# build_readme(${CMAKE_CURRENT_SOURCE_DIR} ${TOOL_NAME})
