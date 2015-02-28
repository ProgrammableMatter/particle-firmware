# @author: Raoul Rubien 2015

cmake_minimum_required(VERSION 2.6)

Project(Atmega)

SET(BINARY "programmable_particle_tests.elf")

add_subdirectory(scripts)
add_subdirectory(libs)
add_subdirectory(main)
