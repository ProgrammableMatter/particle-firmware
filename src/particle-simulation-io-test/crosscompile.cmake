# @author Raoul Rubien 2016

include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/cpu_clock_8000000.cmake)
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/cpu_m16.cmake)
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/compile_settings_release.cmake)
SET(COPT "-Os -fwhole-program -fno-inline-small-functions -fno-inline")
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/compile_settings_global.cmake)
