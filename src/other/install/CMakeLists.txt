# We create an specific target to deploy configuration stuff
add_custom_target(qb_config ALL COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/qb_config.cmake)
