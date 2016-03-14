# @author: Raoul Rubien 2016

include(${PROJECT_SOURCE_DIR}/crosscompile.cmake)

include_directories(
        #${CMAKE_CURRENT_SOURCE_DIR}/../
)

add_library(${PROJECT_NAME}_simulation
        SimulationUtils.c)

