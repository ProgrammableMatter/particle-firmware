# @author Raoul Rubien 2016
cmake_minimum_required(VERSION 2.6)

Project(ParticleReceptionSimulation)

SET(BINARY "${PROJECT_NAME}.elf")

add_subdirectory(libs)
include(crosscompile.cmake)
add_subdirectory(main/avrora)
add_subdirectory(main)
