# @author: Raoul Rubien 2015

#include(${Atmega_SOURCE_DIR}/crosscompile.cmake)

set(SIMULATOR "${CMAKE_CURRENT_SOURCE_DIR}/avrora.sh")
set(HEX_FILE "${CMAKE_BINARY_DIR}/${BINARY}")


add_custom_command(OUTPUT avrora_analyze_stack
    COMMAND ${SIMULATOR} -action=analyze-stack ${HEX_FILE}
)
add_custom_target(avrora-analyze-stack DEPENDS avrora_analyze_stack)


add_custom_command(OUTPUT avrora_control_flow_graph
    COMMAND ${SIMULATOR} -action=cfg -output=dot -file=cfg.dot ${HEX_FILE} && xdot cfg.dot
)
add_custom_target(avrora-cfg DEPENDS avrora_control_flow_graph)


add_custom_command(OUTPUT avrora_elf_dump
    COMMAND ${SIMULATOR} -action=elf-dump ${HEX_FILE}
)
add_custom_target(avrora-elf-dump DEPENDS avrora_elf_dump)


add_custom_command(OUTPUT avrora_isea
    COMMAND ${SIMULATOR} -action=isea ${HEX_FILE}
)
add_custom_target(avrora-inter-procedural-side-effect-analysis DEPENDS avrora_isea)

add_custom_command(OUTPUT avrora_simulate
    COMMAND ${SIMULATOR} -action=simulate ${HEX_FILE}
)
add_custom_target(avrora-simulate DEPENDS avrora_simulate)

add_custom_command(OUTPUT avrora_test
    COMMAND ${SIMULATOR} -action=test -detail=true ${HEX_FILE}
)
add_custom_target(avrora-test DEPENDS avrora_test)
add_custom_target(test DEPENDS avrora_test)


