# @author: Raoul Rubien 2015

include(${PROJECT_SOURCE_DIR}/crosscompile.cmake)

include_directories(
        ${PROJECT_SOURCE_DIR}/libs
)

add_executable(${BINARY}
        main.c
        )

link_directories(
        #        ${PROJECT_SOURCE_DIR}/libs/uc-core
        #        ${PROJECT_SOURCE_DIR}/libs/simulation
)

target_link_libraries(${BINARY}
        #        ${PROJECT_NAME}_uccore
        #        ${PROJECT_NAME}_simulation
        )
include(${PROJECTS_SOURCE_ROOT}/avr-common/scripts/post_binary_build.cmake)
