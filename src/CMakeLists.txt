# @author Raoul Rubien 2016
cmake_minimum_required(VERSION 2.6)

SET(PROJECTS_SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(particle)
add_subdirectory(particle-initial-io-test)
add_subdirectory(particle-simulation)
add_subdirectory(particle-simulation-io-test)
add_subdirectory(particle-reception-simulation)
add_subdirectory(particle-transmission-simulation)
add_subdirectory(manchester-code-tx-simulation)
add_subdirectory(particle-simulation-heatwires-test)
add_subdirectory(particle-simulation-heatwiresrange-test)
add_subdirectory(particle-simulation-setnewnetworkgeometry-test)
add_subdirectory(particle-simulation-sendheader-test)
add_subdirectory(particle-simulation-heatwiresmode-test)
