# @author Raoul Rubien 16.07.2015
cmake_minimum_required(VERSION 2.6)

Project(ParticleSimulationHeatWiresCommandTest)

SET(BINARY "${PROJECT_NAME}.elf")

add_subdirectory(libs)
add_subdirectory(main/avrora)
add_subdirectory(main)
