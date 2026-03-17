#pragma once

/* Created by Gordie Novak on 3/16/26.
 * Purpose: 
 */


#ifdef OpenGL_Core
#include <external/glad4/glad/glad.h>
#define GAN_gladLoadGL gladLoadGLLoader
#else
#include <external/glad_es/glad/glad.h>
#define GAN_gladLoadGL gladLoadGLES2Loader
#endif