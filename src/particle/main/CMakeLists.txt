# @author: Raoul Rubien 2011

include(${Atmega_SOURCE_DIR}/crosscompile.cmake)

include_directories(
    ${Atmega_SOURCE_DIR}/libs
)

add_executable(${BINARY}
    DefaultIsr.cpp 
    main.cpp
)


LINK_DIRECTORIES(
    ${Atmega_SOURCE_DIR}/libs/uc-core
    ${Atmega_SOURCE_DIR}/libs/common
)

target_link_libraries(${BINARY}
    uccore
    common
)


include(${Atmega_SOURCE_DIR}/scripts/post_binary_build.cmake)
