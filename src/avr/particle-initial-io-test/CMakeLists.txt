# @author Raoul Rubien 2016

cmake_minimum_required(VERSION 2.6)

Project(ParticleInitialIoTest)

SET(BINARY "${PROJECT_NAME}.elf")

include(debugger.cmake)
include(programmer.cmake)

add_subdirectory(libs)
add_subdirectory(scripts)
add_subdirectory(main)
