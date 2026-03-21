#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>

#include "../SDL_API.h"
#include "GL_Init.h"

GLuint vao;
GLuint program;
gan::Window winGL = gan::Window::makeGL("My GL Context", {340, 300}, gan::WindowFloatOnTop | gan::WindowResizable);
gan::Clock timer = gan::Clock::create();

SDL_AppResult SDL_AppInit(void** userdata, int argc, char* argv[]) {
    int x, y;

    winGL.setPosition({560, 300});
    const float vertices[] = {
        -1.f,-1.f,0.f,
        -1.f, 1.f,0.f,
         1.f, 1.f,0.f,
        -1.f, -1.f, 0.f,
        1.f, -1.f, 0.f,
        1.f, 1.f, 0.0,
    };

    vao = create_GL_vertex_array(vertices, 6);
    program = create_GL_program("assets/vshader.vert", "assets/fshader.frag");

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* userdata) {

    static uint32_t tick = SDL_GetTicks();
    const uint32_t new_tick = SDL_GetTicks();
    const uint32_t dt = new_tick - tick;
    tick = new_tick;


    glClear(GL_COLOR_BUFFER_BIT);

    auto uniform = glGetUniformLocation(program, "uResolution");
    auto uniform2 = glGetUniformLocation(program, "uMousepos");
    static int w, h;
    static bool _ = SDL_GetWindowSizeInPixels(winGL, &w ,&h);
    int width, height;
    SDL_GetWindowSizeInPixels(winGL, &width, &height);

    glViewport(0, 0, width, height);
    glUniform2f(uniform, float(width), float(height));
    float x, y;
    SDL_GetMouseState(&x, &y);
    winGL.normalizeMousePosToWindow(x, y);

    static float tx = x, ty = y;

    tx += (x-tx)*0.08*dt;
    ty += (y-ty)*0.08;


    if (w != width || h != height) {
        std::cout << "change: " << width << "," << height << std::endl;
        w = width;
        h = height;
    }

    glUniform2f(uniform2, tx, ty);

    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);


    SDL_GL_SwapWindow(winGL);

    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppEvent(void* userdata, SDL_Event* event) {
    switch (event->type) {
    case SDL_EVENT_QUIT:
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        return SDL_APP_SUCCESS;
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* userdata, SDL_AppResult result) {}


