# @author: Raoul Rubien 2015

include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/cpu_clock_8000000.cmake)
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/cpu_m16.cmake)
SET(DEFINED_MACROS "-DSIMULATION=true ${DEFINED_MACROS}")
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/compile_settings_debug.cmake)
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/compile_settings_global.cmake)
