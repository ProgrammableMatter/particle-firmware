# @author: Raoul Rubien 2011

#if (${PROJECT_NAME} STREQUAL "Commons")
#
#else (${PROJECT_NAME} !STREQUAL "Commons")
#    include(${PROJECT_SOURCE_DIR}/programmer.cmake)
#    include(${PROJECT_SOURCE_DIR}/debugger.cmake)
#
#    include(${CMAKE_SOURCE_DIR}/custom_cmake_targets.cmake)
#endif (${PROJECT_NAME} STREQUAL "Commons")