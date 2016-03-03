# @author: Raoul Rubien 2011

# post binary build hack
# ${BINARY]} must be set before.


# symlinking binary for convenience (independend if target is up to date or not)
#execute_process(WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#         COMMAND ln -sfT ${CMAKE_CURRENT_BINARY_DIR}/${BINARY} ${CMAKE_BINARY_DIR}/${BINARY}
##        COMMAND ln -sf ${CMAKE_CURRENT_BINARY_DIR}/${BINARY} ${CMAKE_CURRENT_BINARY_DIR}/main.elf
#         RESULT_VARIABLE ret_var
#         OUTPUT_VARIABLE output_var
#)


# show stats about binary
add_custom_command (TARGET ${BINARY}
         POST_BUILD
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
         #COMMAND ln -sfT ${CMAKE_CURRENT_BINARY_DIR}/${BINARY} ${CMAKE_BINARY_DIR}/${BINARY}
         COMMENT "${BINARY} built for [${CMCU}]@[${CDEFS}]:"
         COMMAND echo "MD5" && md5sum ./${BINARY} | cut -d' ' -f1
         COMMAND echo "filesize on system" && ls -l ./${BINARY} | cut -d' ' -f5 && echo ""
#        COMMAND avr-size --format=avr --mcu=${CMCU} ${BINARY}
         COMMAND avr-size --format=sysv ${BINARY}
)

