# @author: Raoul Rubien 2016

include(${PROJECT_SOURCE_DIR}/crosscompile.cmake)

include_directories(
        ${PROJECT_SOURCE_DIR}/libs
)

add_executable(${BINARY}
        main.c
        )

link_directories(
        ${PROJECT_SOURCE_DIR}/libs/uc-core
)

target_link_libraries(${BINARY}
        ${PROJECT_NAME}_uccore
        )

include(${PROJECTS_SOURCE_ROOT}/avr-common/scripts/post_binary_build.cmake)
