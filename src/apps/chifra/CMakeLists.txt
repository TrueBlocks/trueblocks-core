# minimum cmake version supported
cmake_minimum_required (VERSION 3.0)

# application project
project (chifra)

function(ADD_GO_INSTALLABLE_PROGRAM NAME MAIN_SRC)
  get_filename_component(MAIN_SRC_ABS ${MAIN_SRC} ABSOLUTE)

  add_custom_target(${NAME})
  add_custom_command(TARGET ${NAME}
                    COMMAND go build
                    -o "${BIN_DIR}/${NAME}"
                    ${CMAKE_GO_FLAGS} ${MAIN_SRC}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                    DEPENDS ${MAIN_SRC_ABS} ${MAIN_SRC})
  add_custom_target(${NAME}_all ALL DEPENDS ${NAME})
endfunction(ADD_GO_INSTALLABLE_PROGRAM)

ADD_GO_INSTALLABLE_PROGRAM(chifra *.go)

# Output
set(EXECUTABLE_OUTPUT_PATH "${BIN_DIR}")

# Define the executable to be generated
set(TOOL_NAME "chifra")
set(PROJ_NAME "apps")

# Testing
# Define paths to test folder and gold folder
set(TEST_EXE "${EXECUTABLE_OUTPUT_PATH}/${TOOL_NAME}")
set(TEST_PATH "${TEST_PATH}/${PROJ_NAME}/${TOOL_NAME}")
set(GOLD_PATH "${GOLD_PATH}/${PROJ_NAME}/${TOOL_NAME}")
