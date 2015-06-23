# @author: Raoul Rubien 2015

#create symlinks to script files

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink 
        ${CMAKE_CURRENT_SOURCE_DIR}/simulate-with-pipes.sh
        ${CMAKE_CURRENT_BINARY_DIR}/simulate-with-pipes.sh
)
