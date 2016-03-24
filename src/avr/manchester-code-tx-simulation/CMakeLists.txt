# @author: Raoul Rubien 2015
cmake_minimum_required(VERSION 2.6)

Project(ManchesterCodeTxSimulation)

SET(BINARY "${PROJECT_NAME}.elf")

include(crosscompile.cmake)
#include(debugger.cmake)
#include(programmer.cmake)

#add_subdirectory(scripts)
add_subdirectory(libs)
add_subdirectory(main/avrora)
add_subdirectory(main)
