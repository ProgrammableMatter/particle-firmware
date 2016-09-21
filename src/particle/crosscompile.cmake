# @author Raoul Rubien 2015

include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/cpu_clock_8000000.cmake)
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/cpu_attiny1634.cmake)
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/programmer_target_t1634.cmake)
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/compile_settings_release.cmake)
# for simulation purpose
#SET(COPT "-Os -fno-inline-small-functions -fno-inline")
#SET(COPT "-O1")  # for simulation purpose
# for real MCU
#SET(COPT "-O3 -fwhole-program")
SET(COPT "-O2 -fwhole-program")
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/compile_settings_global.cmake)
