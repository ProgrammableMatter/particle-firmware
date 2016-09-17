# @author Raoul Rubien 2016

include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/cpu_clock_8000000.cmake)
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/cpu_m16.cmake)
SET(DEFINED_MACROS "-DSIMULATION=true ${DEFINED_MACROS}")
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/compile_settings_debug.cmake)
# for simulation purpose
SET(COPT "-Os -fno-inline-small-functions -fno-inline")
#SET(COPT "-O1")  # for simulation purpose
# for real MCU
#SET(COPT "-O3 -fwhole-program")
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/compile_settings_global.cmake)
