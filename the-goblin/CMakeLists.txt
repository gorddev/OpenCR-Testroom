cmake_minimum_required(VERSION 4.0)
project(CRProject)

set(CMAKE_CXX_STANDARD 11)

include_directories(
    ${CMAKE_SOURCE_DIR}
)

add_executable(CRProject
    the-goblin.ino
    clion_config/clion_config_file.cpp
)

target_include_directories(CRProject PRIVATE
    ${CMAKE_SOURCE_DIR}
)