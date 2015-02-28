# @author: Raoul Rubien 2015

set(SIMULATOR "${CMAKE_CURRENT_SOURCE_DIR}/avrora.sh")
set(TRANSLATOR "${CMAKE_CURRENT_SOURCE_DIR}/elf-translator.sh")
set(HEX_FILE "${CMAKE_BINARY_DIR}/${BINARY}")
set(ELF_DUMP "${CMAKE_BINARY_DIR}/elf.dump")
set(CFG_DOT "${CMAKE_BINARY_DIR}/cfg.dot")
set(PRETTY_CFG_DOT "${CMAKE_BINARY_DIR}/pretty-cfg.dot")


add_custom_command(OUTPUT avrora_analyze_stack
    COMMAND ${SIMULATOR} -action=analyze-stack ${HEX_FILE}
)
add_custom_target(avrora-analyze-stack DEPENDS avrora_analyze_stack)


add_custom_command(OUTPUT create_elf_dump
    COMMAND readelf -as ${CMAKE_BINARY_DIR}/${BINARY} > ${ELF_DUMP}
)
add_custom_command(OUTPUT avrora_control_flow_graph
    COMMAND ${SIMULATOR} -action=cfg -output=dot -file=${CFG_DOT} ${HEX_FILE} && ${TRANSLATOR} ${ELF_DUMP} ${CFG_DOT} && xdot ${PRETTY_CFG_DOT}
)
add_custom_target(avrora-cfg DEPENDS create_elf_dump avrora_control_flow_graph)


add_custom_command(OUTPUT avrora_elf_dump
    COMMAND ${SIMULATOR} -action=elf-dump ${HEX_FILE}
)
add_custom_target(avrora-elf-dump DEPENDS avrora_elf_dump)


#add_custom_command(OUTPUT avrora_isea
#    COMMAND ${SIMULATOR} -action=isea ${HEX_FILE}
#)
#add_custom_target(avrora-inter-procedural-side-effect-analysis DEPENDS avrora_isea)

add_custom_command(OUTPUT avrora_simulate
    COMMAND ${SIMULATOR} -action=simulate -monitors=call-profile,stack,interrupts,break ${HEX_FILE}
)
add_custom_target(avrora-simulate DEPENDS avrora_simulate)

#add_custom_command(OUTPUT avrora_test
#    COMMAND ${SIMULATOR} -action=test -detail=true ${HEX_FILE}
#)
#add_custom_target(avrora-test DEPENDS avrora_test)
#add_custom_target(test DEPENDS avrora_test)


