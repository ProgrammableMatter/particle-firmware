# @author Raoul Rubien 2015

include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/cpu_clock_8000000.cmake)
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/cpu_attiny1634.cmake)
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/programmer_target_t1634.cmake)
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/compile_settings_release.cmake)
SET(COPT "-Os -fwhole-program -fno-inline-small-functions")
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/compile_settings_global.cmake)
