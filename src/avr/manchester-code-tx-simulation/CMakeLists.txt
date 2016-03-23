# @author: Raoul Rubien 2015
cmake_minimum_required(VERSION 2.6)

Project(ManchesterCodeTxSimulation)

SET(BINARY "${PROJECT_NAME}.elf")

include(crosscompile.cmake)

add_subdirectory(main)
