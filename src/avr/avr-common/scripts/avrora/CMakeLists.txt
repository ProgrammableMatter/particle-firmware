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
add_custom_target(${PROJECT_NAME}_avrora-analyze-stack DEPENDS avrora_analyze_stack ${PROJECT_NAME})


add_custom_command(OUTPUT create_elf_dump
        COMMAND readelf -as ${CMAKE_BINARY_DIR}/${BINARY} > ${ELF_DUMP}
        )
add_custom_command(OUTPUT avrora_control_flow_graph
        COMMAND ${SIMULATOR} -action=cfg -output=dot -file=${CFG_DOT} ${HEX_FILE} && ${TRANSLATOR} ${ELF_DUMP} ${CFG_DOT} && xdot ${PRETTY_CFG_DOT}
        )
add_custom_target(${PROJECT_NAME}_avrora-cfg DEPENDS create_elf_dump avrora_control_flow_graph ${PROJECT_NAME})


add_custom_command(OUTPUT avrora_elf_dump
        COMMAND ${SIMULATOR} -action=elf-dump ${HEX_FILE}
        )
add_custom_target(${PROJECT_NAME}_avrora-elf-dump DEPENDS avrora_elf_dump ${PROJECT_NAME})


add_custom_command(OUTPUT avrora_isea
    COMMAND ${SIMULATOR} -action=isea ${HEX_FILE}
)
add_custom_target(${PROJECT_NAME}_avrora-inter-procedural-side-effect-analysis DEPENDS avrora_isea ${PROJECT_NAME})

add_custom_command(OUTPUT avrora_simulate
        COMMAND
        ${SIMULATOR}
        -banner=false
        -status-timing=true
#        -verbose=loader,atmel.flash
        -verbose=all
        -seconds-precision=11
        -action=simulate
#           -simulation=sensor-network
            -simulation=particle-network
                -nodecount=2
#                -seconds=190E-4
                -seconds=1000E-6
                -report-seconds=true
                -platform=particle
                -arch=avr
                -clockspeed=8000000
#                -monitors=calls,call-profile,profile,stack,interrupts,memory,break,leds,real-time,retaddr
#                -monitors=calls,call-profile,profile,stack,interrupts,memory,break,leds,retaddr
                -monitors=calls,retaddr,particle,interrupts,memory,break
#                    -ioregs=all
#                -monitors=calls,retaddr,particle
#                    -low-addresses=true
                    -dump-writes=true
                    -show-interrupts=true
                    -invocations-only=false
                    -low-addresses=true
                    -particle-log-file=true
                    -particle-facets=state,break,wires
                -input=elf
            -throughput=true
        ${HEX_FILE})
add_custom_target(${PROJECT_NAME}_avrora-simulate DEPENDS avrora_simulate ${PROJECT_NAME})


