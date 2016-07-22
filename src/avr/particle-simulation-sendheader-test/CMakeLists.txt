# @author Raoul Rubien 21.07.2016

cmake_minimum_required(VERSION 2.6)

Project(ParticleSimulationSendheaderTest)

SET(BINARY "${PROJECT_NAME}.elf")

add_subdirectory(libs)
add_subdirectory(main/avrora)
add_subdirectory(main)
