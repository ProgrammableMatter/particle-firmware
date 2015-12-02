# @author: Raoul Rubien 2015
cmake_minimum_required(VERSION 2.6)

SET(PROJECTS_SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(particle)
add_subdirectory(particle-initial-io-test)
add_subdirectory(particle-simulation)