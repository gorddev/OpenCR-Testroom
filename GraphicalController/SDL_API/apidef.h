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

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef bool b8;
