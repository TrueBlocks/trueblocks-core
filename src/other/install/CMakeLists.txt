# We create an specific target to deploy configuration stuff
add_custom_target(init_configs ALL COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/init_configs.cmake)
