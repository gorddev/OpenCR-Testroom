cmake_minimum_required(VERSION 3.29)
project(GraphicalController)

set(CMAKE_CXX_STANDARD 20)

if (LINUX)
    set(OpenGL_ES ON)
else()
    set(OpenGL_ES OFF)
endif()

# we have to include fetch content to we get repos from git
include(FetchContent)

find_package(SDL3)

####################
# FetchContent Below
####################

if(NOT SDL3_FOUND)
    # ..........................
    #############################
    # SDL 3
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.4.x
    )
    FetchContent_MakeAvailable(SDL3)
    #############################
endif()

#########################
# Serial Compilation Below
#########################

if(APPLE)
    message("Compiling serial library for Apple")
    add_library(Serial STATIC
        ${CMAKE_SOURCE_DIR}/external/serial/src/serial.cc
        ${CMAKE_SOURCE_DIR}/external/serial/src/impl/unix.cc
        ${CMAKE_SOURCE_DIR}/external/serial/src/impl/list_ports/list_ports_osx.cc
    )
elseif(WIN32)
    message("Compiling serial library for windows")
    add_library(Serial STATIC
        ${CMAKE_SOURCE_DIR}/external/serial/src/serial.cc
        ${CMAKE_SOURCE_DIR}/external/serial/src/impl/win.cc
        ${CMAKE_SOURCE_DIR}/external/serial/src/impl/list_ports/list_ports_win.cc
    )
else()
    message("Compiling serial library for linux")
    add_library(Serial STATIC
        ${CMAKE_SOURCE_DIR}/external/serial/src/serial.cc
        ${CMAKE_SOURCE_DIR}/external/serial/src/impl/unix.cc
        ${CMAKE_SOURCE_DIR}/external/serial/src/impl/list_ports/list_ports_linux.cc
    )
endif()

target_include_directories(Serial PRIVATE
    ${CMAKE_SOURCE_DIR}/external/serial/src
    ${CMAKE_SOURCE_DIR}/external/serial/include
)

#########################
# Main compilation
#########################
add_executable(GraphicalController
    main.cpp
    goblin/Goblin-Core/GoblinCore.cpp
)

target_include_directories(GraphicalController PRIVATE
    ${imgui_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external/serial/include
    ${CMAKE_SOURCE_DIR}/goblin
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/SDL_API
    ${CMAKE_SOURCE_DIR}
)

add_subdirectory(SDL_API)

target_link_libraries(GraphicalController
    SDL3::SDL3
    Serial
    SDL_API
)

if(NOT OpenGL_ES)
    target_compile_options(GraphicalController PRIVATE
        -DOpenGL_Core
    )
else()
    target_compile_options(GraphicalController PRIVATE
        -DOpenGL_ES
    )
endif()


if(APPLE)
    target_include_directories(GraphicalController PRIVATE
        ${CMAKE_SOURCE_DIR}/external/glad4
    )
else()
    target_include_directories(GraphicalController PRIVATE
        ${CMAKE_SOURCE_DIR}/external/glad_es
    )
endif ()
