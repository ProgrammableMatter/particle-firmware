# @author: Raoul Rubien 2015
cmake_minimum_required(VERSION 2.6)

Project(ParticleUnitTest)

SET(BINARY "${PROJECT_NAME}.elf")

add_subdirectory(simulavr)
add_subdirectory(scripts)
add_subdirectory(main)
