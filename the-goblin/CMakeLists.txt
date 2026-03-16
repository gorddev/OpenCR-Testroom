cmake_minimum_required(VERSION 4.0)
project(CRProject)

set(CMAKE_CXX_STANDARD 11)

include_directories(
    ${CMAKE_SOURCE_DIR}
)

add_executable(CRProject
    main.cpp
    the-goblin.ino
    the-goblin.ino
)

target_include_directories(CRProject PRIVATE
    ${CMAKE_SOURCE_DIR}
)