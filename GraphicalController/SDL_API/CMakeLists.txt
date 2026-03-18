cmake_minimum_required(VERSION 3.29)
project(SDL_API)

set(CMAKE_CXX_STANDARD 20)

option(SDL_Render_OpenGL_Core "Whether Embedded Systems lib is enabled" ON)

if (SDL_Render_OpenGL_Core)
    message("Standard OpenGL 4.1 enabled. OpenGL_ES set to OFF.")
else()
    message("OpenGL_ES 3.0 enabled. Open_GL Core 4.1 disabled.")
endif()

# we have to include fetch content to we get repos from git
include(FetchContent)

# requires SDL3
find_package(SDL3 REQUIRED)

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

####################
# ImGUI below.
####################

#############################
# Dear SDL_API_ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG master
)
FetchContent_MakeAvailable(imgui)
#############################

add_library(SDL_API_ImGui STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends/imgui_impl_sdl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(SDL_API_ImGui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
)

target_link_libraries(SDL_API_ImGui PRIVATE
    SDL3::SDL3
)

# End SDL_API_ImGui compilation

#########################
# Glad Compilation
#########################

if(SDL_Render_OpenGL_Core)
    add_library(SDL_API_GLAD STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glad4/glad/glad.c
    )
    target_include_directories(SDL_API_GLAD PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glad4
    )
else()
    add_library(SDL_API_GLAD STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glad_es/glad/glad.c
    )
    target_include_directories(SDL_API_GLAD PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glad_es
    )
endif()


#########################
# Main compilation
#########################
add_library(SDL_API
   window/Window.cpp
)

target_include_directories(SDL_API PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/SDL_API
    ${CMAKE_CURRENT_SOURCE_DIR}/external
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SDL_API
    SDL3::SDL3
    SDL_API_ImGui
    SDL_API_GLAD
)


if(SDL_Render_OpenGL_Core)
    target_compile_options(SDL_API PRIVATE
        -DSDL_API_OpenGL_Core
    )
else()
    target_compile_options(SDL_API PRIVATE
        -DSDL_API_OpenGL_ES
    )
endif()


if(SDL_API_OpenGL_ES)
    message("Including glad4   directories")
    target_include_directories(SDL_API PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glad4
    )
else()
    message("Including glades   directories")
    target_include_directories(SDL_API PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glad_es
    )
endif ()


add_executable(Example
    SDL_Example.cpp
)

target_include_directories(Example PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(Example PRIVATE
    SDL_API
)











