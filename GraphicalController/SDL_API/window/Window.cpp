#include <SDL3/SDL.h>

#include "../types/vec2.hpp"

#include <apidef.h>

#ifdef GAN_DEBUG
#include <iostream>
#include <ostream>
#endif

#include "./Window.hpp"
#include "../errors/gan_err.hpp"

// created by gordie feb 16th. implementation for window

using namespace gan;

Window::Window(SDL_Window* win, WindowProperty flags, const SDL_WindowID id, SDL_GLContext gl, dim2 dim)
    : sdl_window(win), flags(flags), id(id), gl_context(gl), dimensions(dim) {}


Window Window::make(const char windowName[], const dim2 dim, const WindowProperty flags)
{
    ensure_SDL_init();

    SDL_Window* sdl_window = SDL_CreateWindow(windowName, static_cast<int>(dim.w), static_cast<int>(dim.h), flags);

    if (!sdl_window) {
        err::panic("Window::Window()", "Failed to make window with error: ",  SDL_GetError());
    }

    return Window{
        sdl_window,
        flags,
        SDL_GetWindowID(sdl_window),
        nullptr,
        dim
    };
}

Window Window::makeGL(const char windowName[], dim2 dim, WindowProperty flags)
{
    ensure_SDL_init();

    #ifdef SDL_API_OpenGL_Core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    #else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    #endif
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    SDL_Window* sdl_window = SDL_CreateWindow(windowName, dim.w, dim.h, flags | SDL_WINDOW_OPENGL);

    if (!sdl_window) {
        err::panic("Window::Window()", "Failed to make window with error: ",  SDL_GetError());
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(sdl_window);

    if (!gl_context)
        err::panic("Window::Window()", "Failed to make OpenGL context with error: ",  SDL_GetError());

    GAN_gladLoadGL((GLADloadproc)SDL_GL_GetProcAddress);

    printf("OpenGL Context Initialized: %s\n", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

    SDL_ClearError();

    SDL_GL_MakeCurrent(sdl_window, gl_context);
    SDL_GL_SetSwapInterval(1);

    int x, y;
    SDL_GetWindowSizeInPixels(sdl_window, &x, &y);

    glViewport(0, 0, x, y);
    glClearColor(0.1f, 0.1f, 0.1f, 0.3f);
    return Window {
        sdl_window,
        flags,
        SDL_GetWindowID(sdl_window),
        gl_context,
        dim
    };
}

Window::~Window() {
    SDL_DestroyWindow(sdl_window);
    sdl_window = nullptr;
    flags = WindowDestroyed;
}

void Window::normalizeMousePosToWindow(float& x, float& y) const {
    int wx, wy;
    SDL_GetWindowSizeInPixels(sdl_window, &wx, &wy);

    x = (x-wx/2.f)/dimensions.w;
    y = ((y-wy/2.f)/dimensions.h);
}

Window::operator SDL_Window*() const noexcept {
    return sdl_window;
}

void Window::setDimensions(const dim2 dim) const {
    SDL_SetWindowSize(sdl_window, dim.w, dim.h);
}

void Window::setWidth(uint32_t width) const {
    SDL_SetWindowSize(sdl_window, width, dimensions.h);
}

void Window::setHeight(uint32_t height) const {
    SDL_SetWindowSize(sdl_window, dimensions.w, height);
}

void Window::setPosition(const vec2 pos) const {
    SDL_SetWindowPosition(sdl_window, pos.x, pos.y);
}

void Window::setFullscreen()  {
    flags |= WindowFullscreen;
    SDL_SetWindowFullscreen(sdl_window, true);
}

void Window::setWindowed() {
    flags &= ~WindowFullscreen;
    SDL_SetWindowFullscreen(sdl_window, false);
}

void Window::setResizable(const bool b) {
    if (b)  flags |= WindowResizable;
    else    flags &= ~WindowResizable;
    SDL_SetWindowResizable(sdl_window, b);
}

void Window::setFloatOnTop(const bool b) {
    if (b)  flags |= WindowFloatOnTop;
    else    flags &= ~WindowFloatOnTop;
    SDL_SetWindowAlwaysOnTop(sdl_window, b);
}

void Window::setMouseGrab(const bool b) {
    if (b)  flags |= WindowMouseConfined;
    else    flags &= ~WindowMouseConfined;
    SDL_SetWindowMouseGrab(sdl_window, b);
}

void Window::setMouseLocking(const bool hidden) {
    if (hidden)  flags |= WindowMouseHidden;
    else        flags &= ~WindowMouseHidden;
    SDL_SetWindowRelativeMouseMode(sdl_window, hidden);
}

void Window::setKeyboardGrab(const bool b) {
    if (b)  flags |= WindowKeyboardGrabbed;
    else    flags &= ~WindowKeyboardGrabbed;
    SDL_SetWindowMouseGrab(sdl_window, b);
}

void Window::setIcon(const char pathToImage[]) {
    SDL_Surface* surf; //= IMG_Load(pathToImage);
    #ifdef GAN_DEBUG
    if (!surf)
        std::cout << "Failed to load image: " << pathToImage << ".\n" << SDL_GetError() << std::endl;
    #endif
    SDL_SetWindowIcon(sdl_window, surf);
    SDL_DestroySurface(surf);
}

void Window::setName(const char name[]) const {
    SDL_SetWindowTitle(sdl_window, name);
}

void Window::hide() {
    flags |= WindowHidden;
    SDL_HideWindow(sdl_window);
}

void Window::show() {
    flags &= ~WindowHidden;
    SDL_ShowWindow(sdl_window);
}

void Window::setOpacity(const float opacity) const {
    if (flags & WindowTransparent)
        SDL_SetWindowOpacity(sdl_window, opacity);
    else
        err::panic("gan::Window::setOpacity()",
            "Cannot set window opacity, as flag 'WindowTransparent'"
            "was not enabled at launch.");
}

bool Window::isFullscreen() const {
    return flags & WindowFullscreen;
}

bool Window::isWindowed() const {
    return !(flags & WindowFullscreen);
}

bool Window::isHidden() const {
    return flags & WindowHidden;
}

bool Window::hasProperty(const WindowProperty flag) const {
    return flags & flag;
}

bool Window::isResizable() const {
    return flags & WindowResizable;
}

bool Window::isFloatOnTop() const {
    return flags & WindowFloatOnTop;
}

bool Window::isMouseConfined() const {
    return flags & WindowMouseConfined;
}

bool Window::isKeyboardGrab() const {
    return flags & WindowKeyboardGrabbed;
}

bool Window::isTransparent() const {
    return flags & WindowTransparent;
}

bool Window::isMouseLocked() const {
    return flags & WindowMouseHidden;
}

bool Window::isFocused() const {
    return flags & WindowFocused;
}

float Window::getOpacity() const noexcept {
    return SDL_GetWindowOpacity(sdl_window);
}

dim2 Window::getDimensions() const noexcept {
    return dimensions;
}

uint32_t Window::getWidth() const noexcept {
    return dimensions.w;
}

uint32_t Window::getHeight() const noexcept {
    return dimensions.h;
}

vec2 Window::getPosition() const noexcept {
    int x, y;
    SDL_GetWindowPosition(sdl_window, &x, &y);
    return {static_cast<float>(x), static_cast<float>(y)};
}

uint32_t Window::getWindowId() const noexcept {
    return id;
}

SDL_GLContext Window::getGLContext() const noexcept {
    return gl_context;
}

dim2 Window::getWindowPixelSize() const noexcept {
    int x, y;
    SDL_GetWindowSizeInPixels(sdl_window, &x, &y);
    return {x, y};
}

void Window::setGLClearColor(float r, float g, float b, float a) const noexcept {
    if (gl_context) {
        SDL_GL_MakeCurrent(sdl_window, gl_context);
        glClearColor(r, g, b, a);
    }
}

void Window::on_resize(SDL_Event& e) noexcept {
    const int w = e.window.data1;
    const int h = e.window.data2;
    dimensions = dim2{(int)w, (int)h};
}



