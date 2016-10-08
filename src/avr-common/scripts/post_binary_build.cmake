# @author Raoul Rubien 2016

# show stats about binary
add_custom_command(TARGET ${BINARY}
        POST_BUILD
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "${BINARY} built for [${CMCU}]@[${CDEFS}] CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}:"
        COMMAND echo "MD5" && md5sum ./${BINARY} | cut -d' ' -f1
        COMMAND echo "filesize on system" && ls -l ./${BINARY} | cut -d' ' -f5 && echo ""
        COMMAND avr-size --format=sysv ${BINARY}
        )

