# @author: Raoul Rubien 2011

cmake_minimum_required(VERSION 2.6)

Project(Atmega)

SET(BINARY "programmable_particle.elf")

add_subdirectory(scripts)
add_subdirectory(libs)
add_subdirectory(main)
