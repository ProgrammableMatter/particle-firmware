# @author: Raoul Rubien 2011

include(${Atmega_SOURCE_DIR}/programmer.cmake)
include(${Atmega_SOURCE_DIR}/debugger.cmake)

add_subdirectory(avrora)
add_subdirectory(debug)

SET(WRITE_FLASH  /bin/bash ${CMAKE_CURRENT_BINARY_DIR}/write_flash.sh  ${PROGRAMMER} ${PROGRAMMER_TARGET} ${PROGRAMMER_BAUD} ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS}  ${CMAKE_BINARY_DIR}/${BINARY})
SET(WRITE_FUSE   /bin/bash ${CMAKE_CURRENT_BINARY_DIR}/write_fuse.sh   ${PROGRAMMER} ${PROGRAMMER_TARGET} ${PROGRAMMER_BAUD} ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS}  ${CMAKE_BINARY_DIR}/${BINARY} )
SET(VERIFY_FLASH /bin/bash ${CMAKE_CURRENT_BINARY_DIR}/verify_flash.sh ${PROGRAMMER} ${PROGRAMMER_TARGET} ${PROGRAMMER_BAUD} ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS}  ${CMAKE_BINARY_DIR}/${BINARY}.flash)
SET(ERASE_FLASH  /bin/bash ${CMAKE_CURRENT_BINARY_DIR}/erase_flash.sh  ${PROGRAMMER} ${PROGRAMMER_TARGET} ${PROGRAMMER_BAUD} ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS})


add_custom_command(OUTPUT cat_uart
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/debug_monitor.sh ${DEBUGGER_PORT} ${DEBUGGER_BAUD}
)
add_custom_target(listen DEPENDS cat_uart)

add_custom_command(OUTPUT show_help
    COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/README.md 
)
add_custom_target(info DEPENDS show_help)


add_custom_command(OUTPUT __clear_cmake_cache
    COMMAND rm -f ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND cmake ${CMAKE_SOURCE_DIR}
)

add_custom_command(OUTPUT write_fuse
    # DEPENDS __clear_cmake_cache
    COMMAND ${WRITE_FUSE}
)
add_custom_target(fuse DEPENDS write_fuse)


add_custom_command(OUTPUT write_flash
    # DEPENDS __clear_cmake_cache
    COMMAND ${WRITE_FLASH}        
)
add_custom_target(flash DEPENDS write_flash)


add_custom_command(OUTPUT verify_flash
    # DEPENDS __clear_cmake_cache
    COMMAND ${VERIFY_FLASH}        
)
add_custom_target(verify DEPENDS verify_flash)


add_custom_command(OUTPUT erase_flash
    COMMAND ${ERASE_FLASH}        
)
add_custom_target(erase DEPENDS erase_flash)


#create symlinks to script files
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink 
        ${CMAKE_CURRENT_SOURCE_DIR}/write_flash.sh
        ${CMAKE_CURRENT_BINARY_DIR}/write_flash.sh
)

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink 
        ${CMAKE_CURRENT_SOURCE_DIR}/erase_flash.sh
        ${CMAKE_CURRENT_BINARY_DIR}/erase_flash.sh
)

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink 
        ${CMAKE_CURRENT_SOURCE_DIR}/write_fuse.sh
        ${CMAKE_CURRENT_BINARY_DIR}/write_fuse.sh
)

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink 
        ${CMAKE_CURRENT_SOURCE_DIR}/verify_flash.sh
        ${CMAKE_CURRENT_BINARY_DIR}/verify_flash.sh
)

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink 
        ${CMAKE_CURRENT_SOURCE_DIR}/utils.sh
        ${CMAKE_CURRENT_BINARY_DIR}/utils.sh
)


# symlinks for convenience
#execute_process(WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#        COMMAND ${CMAKE_COMMAND} -E create_symlink 
#        ${CMAKE_SOURCE_DIR}/scripts/write_flash.sh
#        ${CMAKE_BINARY_DIR}/write_flash.sh
#)
#execute_process(WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#        COMMAND ${CMAKE_COMMAND} -E create_symlink 
#        ${CMAKE_SOURCE_DIR}/scripts/utils.sh
#        ${CMAKE_BINARY_DIR}/utils.sh
#)


add_custom_command(OUTPUT read_fuses
    COMMAND 
        echo "hfuse: " && avrdude  -c ${PROGRAMMER} -p ${PROGRAMMER_TARGET} -b ${PROGRAMMER_BAUD} -P ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS} -U hfuse:r:-:b 2>&1 | grep 0b
        && sleep 2 &&
        echo "lfuse: " && avrdude  -c ${PROGRAMMER} -p ${PROGRAMMER_TARGET} -b ${PROGRAMMER_BAUD} -P ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS} -U lfuse:r:-:b 2>&1 | grep 0b
        && sleep 2 &&
        echo "efuse: " && avrdude -c ${PROGRAMMER} -p ${PROGRAMMER_TARGET} -b ${PROGRAMMER_BAUD} -P ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS} -U efuse:r:-:b 2>&1 | grep 0b
)
add_custom_target(rfuses DEPENDS read_fuses)


add_custom_command(OUTPUT read_hfuse
         COMMAND echo "hfuse: " && avrdude -c ${PROGRAMMER} -p ${PROGRAMMER_TARGET} -b ${PROGRAMMER_BAUD} -P ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS} -U hfuse:r:-:b | grep 0b
)
add_custom_target(rhfuse DEPENDS read_hfuse)

add_custom_command(OUTPUT read_lfuse
         COMMAND echo "lfuse: " && avrdude -c ${PROGRAMMER} -p ${PROGRAMMER_TARGET} -b ${PROGRAMMER_BAUD} -P ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS} -U lfuse:r:-:b | grep 0b
)
add_custom_target(rlfuse DEPENDS read_lfuse)


add_custom_command(OUTPUT read_efuse
         COMMAND echo "efuse: " && avrdude -c ${PROGRAMMER} -p ${PROGRAMMER_TARGET} -b ${PROGRAMMER_BAUD} -P ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS} -U efuse:r:-:b | grep 0b
)
add_custom_target(refuse DEPENDS read_efuse)


add_custom_command(OUTPUT terminal_mode 
         COMMAND avrdude -p ${PROGRAMMER_TARGET} -b ${PROGRAMMER_BAUD} -P ${PROGRAMMER_PORT} -c ${PROGRAMMER} ${PROGRAMMER_EXTRA_FLAGS} -t
)
add_custom_target(avrterminal DEPENDS terminal_mode)
