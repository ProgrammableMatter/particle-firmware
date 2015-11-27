# @author: Raoul Rubien 2015

include(${PROJECT_SOURCE_DIR}/crosscompile.cmake)

include_directories(
        ${PROJECT_SOURCE_DIR}/libs
)

add_executable(${BINARY}
        DefaultIsr.c
        main.c
        )

link_directories(
#        ${PROJECT_SOURCE_DIR}/libs/common
        ${PROJECT_SOURCE_DIR}/libs/uc-core
)

target_link_libraries(${BINARY}
#        ${PROJECT_NAME}_common
        ${PROJECT_NAME}_uccore
        )
include(${PROJECTS_SOURCE_ROOT}/avr-common/scripts/post_binary_build.cmake)
