# @author Raoul Rubien 2016

include(${Atmega_SOURCE_DIR}/crosscompile.cmake)

set(SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/start-gdb.sh")

set(HEX_FILE "${CMAKE_BINARY_DIR}/${BINARY}")


add_custom_command(OUTPUT start_gdb_wit_ddd
        COMMAND ${SCRIPT} ${CMCU} ${HEX_FILE}
        )

add_custom_target(${PROJECT_NAME}_ddd DEPENDS start_gdb_wit_ddd)

