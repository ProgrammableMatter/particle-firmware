include(${Atmega_SOURCE_DIR}/crosscompile.cmake)

include_directories(${Atmega_SOURCE_DIR}/utils)

add_library (common
 Trigger.c 
)

