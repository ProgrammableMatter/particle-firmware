# @author: Raoul Rubien 2011

include(${Atmega_SOURCE_DIR}/crosscompile.cmake)

include_directories(${Atmega_SOURCE_DIR}/utils)

add_library (common
    pure_virtual_pita.c
)

