include(${Atmega_SOURCE_DIR}/crosscompile.cmake)

include_directories(
    ${Atmega_SOURCE_DIR}/libs
)

add_executable(${BINARY} 
    main.cpp
)


LINK_DIRECTORIES(
    ${Atmega_SOURCE_DIR}/libs/lcd
)

target_link_libraries(${BINARY}
    lcd
)


include(${Atmega_SOURCE_DIR}/scripts/post_binary_build.cmake)
