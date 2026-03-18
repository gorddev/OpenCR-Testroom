#pragma once

/* Created by Gordie Novak on 3/16/26.
 * Purpose: 
 */


#ifdef SDL_API_OpenGL_Core
#include "external/glad4/glad/glad.h"
#define GAN_gladLoadGL gladLoadGLLoader
#else
#include "external/glad_es/glad/glad.h"
#define GAN_gladLoadGL gladLoadGLES2Loader
#endif

#include <SDL3/SDL.h>
namespace gan {
    inline void ensure_SDL_init() {
        [[maybe_unused]] static bool b = SDL_Init(SDL_INIT_VIDEO);
    }
}