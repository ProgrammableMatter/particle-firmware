# @author: Raoul Rubien 2011

SET(WRITE_FLASH /bin/bash ${PROJECTS_SOURCE_ROOT}/avr-common/scripts/write_flash.sh ${PROGRAMMER} ${PROGRAMMER_TARGET} ${PROGRAMMER_BAUD} ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS} ${CMAKE_BINARY_DIR}/${BINARY} ${POST_PROGRAMMING_COMMAND})
SET(WRITE_FUSE /bin/bash ${PROJECTS_SOURCE_ROOT}/avr-common/scripts/write_fuse.sh ${PROGRAMMER} ${PROGRAMMER_TARGET} ${PROGRAMMER_BAUD} ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS} ${CMAKE_BINARY_DIR}/${BINARY} ${POST_PROGRAMMING_COMMAND})
SET(VERIFY_FLASH /bin/bash ${PROJECTS_SOURCE_ROOT}/avr-common/scripts/verify_flash.sh ${PROGRAMMER} ${PROGRAMMER_TARGET} ${PROGRAMMER_BAUD} ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS} ${CMAKE_BINARY_DIR}/${BINARY}.flash ${POST_PROGRAMMING_COMMAND})
SET(ERASE_FLASH /bin/bash ${PROJECTS_SOURCE_ROOT}/avr-common/scripts/erase_flash.sh ${PROGRAMMER} ${PROGRAMMER_TARGET} ${PROGRAMMER_BAUD} ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS} ${POST_PROGRAMMING_COMMAND})

add_custom_command(OUTPUT cat_uart
        COMMAND ${PROJECTS_SOURCE_ROOT}/avr-common/scripts/debug_monitor.sh ${DEBUGGER_PORT} ${DEBUGGER_BAUD}
        )
add_custom_target(${PROJECT_NAME}_listen DEPENDS cat_uart)

add_custom_command(OUTPUT show_help
        COMMAND cat ${PROJECTS_SOURCE_ROOT}/avr-common/scripts/README.md
        )
add_custom_target(${PROJECT_NAME}_info DEPENDS show_help)

add_custom_command(OUTPUT __clear_cmake_cache
        COMMAND rm -f ${CMAKE_BINARY_DIR}/CMakeCache.txt
        COMMAND cmake ${CMAKE_SOURCE_DIR}
        )

add_custom_command(OUTPUT write_fuse
        # DEPENDS __clear_cmake_cache
        COMMAND ${WRITE_FUSE}
        )
add_custom_target(${PROJECT_NAME}_fuse DEPENDS write_fuse)

add_custom_command(OUTPUT write_flash
        # DEPENDS __clear_cmake_cache
        COMMAND ${WRITE_FLASH}
        )
add_custom_target(${PROJECT_NAME}_flash DEPENDS write_flash)

add_custom_command(OUTPUT verify_flash
        # DEPENDS __clear_cmake_cache
        COMMAND ${VERIFY_FLASH}
        )
add_custom_target(${PROJECT_NAME}_verify DEPENDS verify_flash)

add_custom_command(OUTPUT erase_flash
        COMMAND ${ERASE_FLASH}
        )
add_custom_target(${PROJECT_NAME}_erase DEPENDS erase_flash)

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

add_custom_command(OUTPUT read_fuses
        COMMAND
        echo "hfuse: " && avrdude -c ${PROGRAMMER} -p ${PROGRAMMER_TARGET} -b ${PROGRAMMER_BAUD} -P ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS} -U hfuse:r:-:b 2>&1 | grep 0b
        && sleep 2 &&
        echo "lfuse: " && avrdude -c ${PROGRAMMER} -p ${PROGRAMMER_TARGET} -b ${PROGRAMMER_BAUD} -P ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS} -U lfuse:r:-:b 2>&1 | grep 0b
        && sleep 2 &&
        echo "efuse: " && avrdude -c ${PROGRAMMER} -p ${PROGRAMMER_TARGET} -b ${PROGRAMMER_BAUD} -P ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS} -U efuse:r:-:b 2>&1 | grep 0b
        )
add_custom_target(${PROJECT_NAME}_rfuses DEPENDS read_fuses)

add_custom_command(OUTPUT read_hfuse
        COMMAND echo "hfuse: " && avrdude -c ${PROGRAMMER} -p ${PROGRAMMER_TARGET} -b ${PROGRAMMER_BAUD} -P ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS} -U hfuse:r:-:b | grep 0b
        )
add_custom_target(${PROJECT_NAME}_rhfuse DEPENDS read_hfuse)

add_custom_command(OUTPUT read_lfuse
        COMMAND echo "lfuse: " && avrdude -c ${PROGRAMMER} -p ${PROGRAMMER_TARGET} -b ${PROGRAMMER_BAUD} -P ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS} -U lfuse:r:-:b | grep 0b
        )
add_custom_target(${PROJECT_NAME}_rlfuse DEPENDS read_lfuse)

add_custom_command(OUTPUT read_efuse
        COMMAND echo "efuse: " && avrdude -c ${PROGRAMMER} -p ${PROGRAMMER_TARGET} -b ${PROGRAMMER_BAUD} -P ${PROGRAMMER_PORT} ${PROGRAMMER_EXTRA_FLAGS} -U efuse:r:-:b | grep 0b
        )
add_custom_target(${PROJECT_NAME}_refuse DEPENDS read_efuse)

add_custom_command(OUTPUT terminal_mode
        COMMAND avrdude -p ${PROGRAMMER_TARGET} -b ${PROGRAMMER_BAUD} -P ${PROGRAMMER_PORT} -c ${PROGRAMMER} ${PROGRAMMER_EXTRA_FLAGS} -t
        )
add_custom_target(${PROJECT_NAME}_avrterminal DEPENDS terminal_mode)
