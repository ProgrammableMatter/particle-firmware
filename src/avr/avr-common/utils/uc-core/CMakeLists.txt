# @author: Raoul Rubien 2015

include(${Atmega_SOURCE_DIR}/crosscompile.cmake)

include_directories(${Atmega_SOURCE_DIR}/libs)

add_library (uccore
	Particle.c
	Indicator.c
)

