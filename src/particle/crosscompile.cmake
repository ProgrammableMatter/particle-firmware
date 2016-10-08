# @author Raoul Rubien 2015

include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/cpu_clock_8000000.cmake)
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/cpu_attiny1634.cmake)
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/programmer_target_t1634.cmake)
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/compile_settings_release.cmake)
#SET(COPT "-O1 -fwhole-program")
SET(COPT "-Os -fwhole-program -fno-inline-small-functions")

#SET(PRINTF_LIB_FLOAT "-Wl,-u,vfprintf -lprintf_flt")
SET(PRINTF_LIB_FLOAT "")
include(${PROJECTS_SOURCE_ROOT}/avr-common/targets/compile_settings_global.cmake)
