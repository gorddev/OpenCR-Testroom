#pragma once

/* Created by Gordie Novak on 3/15/26.
 * Purpose: 
 */
#pragma once

#include <optional>
#include <utility>
#include <SDL3/SDL_video.h>

#include "../RenderingSettings.hpp"
#include "../types/dim2.hpp"
#include "../types/vec2.hpp"

// made by gordie novak feb 16th

// new window management system so you can easily adjust window properties


namespace gan {

    class Renderer;

    /// Used to easily update window properties.
    class Window {
        /// Window the renderer draws to.
        SDL_Window *sdl_window;
        /// Internal flags for keeping track of states
        WindowProperty flags;
    public:
        /// Internal id of the window
        const SDL_WindowID id;
    private:
        /// GL context
        SDL_GLContext gl_context;
        /// Internal Window dimensions
        dim2 dimensions;

        Window(SDL_Window*, WindowProperty, SDL_WindowID, SDL_GLContext, dim2);

    public:
        /// Construct with a default width and height
        /// @param windowName The name of the window you want to create
        /// @param dim The width and height of the window.
        /// @param flags Creates specific flags for the window.
        /// @return The Window if successful. @code std::nullopt@endcode otherwise. Call @code GAN_GetError()@endcode
        /// for error information.
        static Window make(const char windowName[], dim2 dim, WindowProperty flags);

        /// Construct with a default width and height with a GL context attatched.
        /// @param windowName The name of the window you want to create
        /// @param dim The width and height of the window.
        /// @param flags Creates specific flags for the window.
        /// @return The Window if successful. @code std::nullopt@endcode otherwise. Call @code GAN_GetError()@endcode
        /// for error information.
        static Window makeGL(const char windowName[], dim2 dim, WindowProperty flags);

        ~Window(); ///< Destructor

        // <><><> Setters <><><>
        void setDimensions(dim2 dim) const;     ///< Sets the window width & height @code {w, h}@endcode (in pixels)
        void setWidth(uint32_t width) const;    ///< Sets the width of the window (in pixels).
        void setHeight(uint32_t height) const;  ///< Sets the height of the window (in pixels).
        void setPosition(vec2 pos) const;       ///< Sets the position of the window (in pixels)
        // *********************
        void setFullscreen();                   ///< Sets the window to fullscreen mode.
        void setWindowed();                     ///< Sets the window into "Windowed" mode.
        void setResizable(bool);                ///< Enables/disables window resizability
        void setFloatOnTop(bool);               ///< Enables/disables the window staying on top of other windows.
        void setMouseGrab(bool);                ///< Confines/frees the mouse to the window
        void setMouseLocking(bool);             ///< Locks/unlocks & hides/unhides the mouse (like in an FPS game).
        void setKeyboardGrab(bool) ;            ///< Locks/unlocks the keyboard to the window.
        void setIcon(const char pathToImage[]); ///< Sets the icon of the application to the image at the given path.
        void setName(const char name[]) const;  ///< Sets the name of the window
        void hide();                            ///< Makes the window hidden if currently visible.
        void show();                            ///< Makes the window visible if currently hidden.
        /** Sets the opacity of the window
          * @note Requires flag @code WindowTransparent@endcode enabled at launch.
          * @warning Not web-safe. Does not work within web browsers.  */
        void setOpacity(float opacity) const;

        // <><><> Getters <><><>
        [[nodiscard]] bool isFloatOnTop() const;    ///< Returns @code true@endcode If the window has priority of display over other windows.
        [[nodiscard]] bool isFocused() const;       ///< Returns @code true@endcode If the window is currently focused.
        [[nodiscard]] bool isFullscreen() const;    ///< Returns @code true@endcode if the window is in fullscreen mode.
        [[nodiscard]] bool isKeyboardGrab() const;  ///< Returns @code true@endcode If the keyboard is currently bound to the window.
        [[nodiscard]] bool isMouseConfined() const; ///< Returns @code true@endcode If the mouse is currently trapped within the window.
        [[nodiscard]] bool isMouseLocked() const;   ///< Returns @code true@endcode If the mouse is currently locked & hidden in the window.
        [[nodiscard]] bool isResizable() const;     ///< Returns @code true@endcode If the window is currently resizable.
        [[nodiscard]] bool isTransparent() const;   ///< Returns @code true@endcode If the window has transparency enabled.
        [[nodiscard]] bool isWindowed() const;      ///< Returns @code true@endcode if the window is in "windowed" mode.
        [[nodiscard]] bool isHidden() const;        ///< Returns @code true@endcode if the window is currently hidden.

        [[nodiscard]] bool hasProperty(WindowProperty) const; ///< Returns @code true@endcode if the window has the specified window property.

        // <><><> Window Size Attributes <><><>
        [[nodiscard]] float     getOpacity() const noexcept;        ///< Returns the opacity of the current window if transparency is enabled.
        [[nodiscard]] dim2      getDimensions() const noexcept;     ///< Returns the dimensions of the window {width, height} (in pixels)
        [[nodiscard]] uint32_t  getWidth() const noexcept;          ///< Returns the width of the current window (in pixels)
        [[nodiscard]] uint32_t  getHeight() const noexcept;         ///< Returns the height of the current window (in pixels)
        [[nodiscard]] vec2      getPosition() const noexcept;       ///< Returns the position of the current window {x, y} (in pixels)
        [[nodiscard]] uint32_t  getWindowId() const noexcept;
        [[nodiscard]] SDL_GLContext getGlContext() const noexcept;  ///< Returns GLContext object

        void on_resize(SDL_Event& e) noexcept;

        void normalizeMousePosToWindow(float &x, float &y) const; ///< Gives mouse position in NDC (-1 to 1)

        operator SDL_Window* () const noexcept; // NOLINT(*-explicit-constructor)
    };
}