# @author: Raoul Rubien 2015

include(${PROJECT_SOURCE_DIR}/crosscompile.cmake)

include_directories(
)

add_executable(${BINARY}
        main.c
        )

link_directories(
)

target_link_libraries(${BINARY}
        )

include(${PROJECTS_SOURCE_ROOT}/avr-common/scripts/post_binary_build.cmake)
