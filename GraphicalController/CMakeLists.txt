cmake_minimum_required(VERSION 3.29)
project(GraphicalController)

set(CMAKE_CXX_STANDARD 20)

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

#############################
# Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG master
)
FetchContent_MakeAvailable(imgui)
#############################


#########################
# ImGui Compilation Below
#########################

add_library(ImGui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(ImGui PRIVATE
    ${imgui_SOURCE_DIR}/backends
    ${imgui_SOURCE_DIR}
)

target_link_libraries(ImGui PRIVATE
    SDL3::SDL3
)

# End ImGui compilation

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
elseif()
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
# Glad Compilation
#########################
if(NOT APPLE)
add_library(GLAD STATIC
    ${CMAKE_SOURCE_DIR}/external/glad_es/glad/glad.c
)
target_include_directories(GLAD PRIVATE
    ${CMAKE_SOURCE_DIR}/external/glad_es
)
else()
add_library(GLAD STATIC
    ${CMAKE_SOURCE_DIR}/external/glad4/glad/glad.c
)
target_include_directories(GLAD PRIVATE
    ${CMAKE_SOURCE_DIR}/external/glad4
)
endif()



#########################
# Main compilation
#########################
add_executable(GraphicalController
    main.cpp
    SDL_API/window/Window.cpp
)

target_include_directories(GraphicalController PRIVATE
    ${imgui_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external/serial/include
    ${CMAKE_SOURCE_DIR}/SDL_API
    ${CMAKE_SOURCE_DIR}/goblin
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(GraphicalController
    SDL3::SDL3
    ImGui
    Serial
    GLAD
)


if(APPLE)
    target_include_directories(GraphicalController PRIVATE
        ${CMAKE_SOURCE_DIR}/external/glad4
    )
else()
    target_include_directories(GraphicalController PRIVATE
        ${CMAKE_SOURCE_DIR}/external/glad_es
    )
endif ()
