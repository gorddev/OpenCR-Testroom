#pragma once

#include "../external/imgui/imgui.h"
#include "../external/imgui/backends/imgui_impl_sdl3.h"
#include"../external/imgui/backends/imgui_impl_opengl3.h"

#ifdef __APPLE__
#include "external/glad4/glad/glad.h"
#else
#include "external/glad_es/glad/glad.h"
#endif

#include "SDL_API/window/Window.hpp"

/* Created by Gordie Novak on 3/16/26.
 * Purpose:
 */

namespace gan {
    class InfoPanel {
    protected:
        ImGuiContext* imgui_context;
        ImGuiIO& imGuiIO;
        gan::Window window;
        dim2 sizeReq;

    public:
        explicit InfoPanel(const char windowName[], WindowProperty prop = WindowFloatOnTop) : imgui_context(ImGui::CreateContext()),
            window(Window::makeGL(windowName, {750, 500}, WindowTransparent | prop)), imGuiIO(ImGui::GetIO())
        {
            //SDL_SetWindowResizable(window, false);
            ImGui_ImplSDL3_InitForOpenGL(window, window.getGLContext());
            #ifdef __APPLE__
            ImGui_ImplOpenGL3_Init("#version 410 core");
            #else
            std::cerr << glGetString(GL_VERSION) << std::endl;
            ImGui_ImplOpenGL3_Init("#version 300 es");
            #endif
            std::cerr << glGetString(GL_VERSION) << std::endl;
        }

        void handleEvent(const SDL_Event &event) {
            ImGui::SetCurrentContext(imgui_context);
            ImGui_ImplSDL3_ProcessEvent(&event);

            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                window.hide();
            }
        }

        virtual ~InfoPanel() {
            ImGui_ImplSDL3_Shutdown();
            ImGui_ImplOpenGL3_Shutdown();
            ImGui::DestroyContext(imgui_context);
            SDL_DestroyWindow(window);
        }

    protected:
        void beginInfoPanel() {
            if (window.isHidden())
                return;

            ImGui::SetCurrentContext(imgui_context);

            // Set up the new frames for each.
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

        }

        void endInfoPanel() const {
            if (window.isHidden())
                return;
            // resize according to window size.
            //resize_window(sizeReq);

            ImGui::End();

            draw();
        }

    private:
        void setup_window() const {
            ImGui::SetNextWindowPos({0,0}, ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(300,200), ImGuiCond_FirstUseEver);
        }

        void resize_window(const dim2& size) const {
            int w, h;
            SDL_GetWindowSizeInPixels(window, &w, &h);
            auto im_size = ImGui::GetWindowSize();
            if (std::abs((int)im_size.x -(int)w) > 1) {
                window.setDimensions({(int)im_size.x, (int)im_size.y});
            }
        }


        void draw() const {
            SDL_GL_MakeCurrent(window, window.getGLContext());
            ImGui::SetCurrentContext(imgui_context);
            ImGui::Render();

            int w, h;
            SDL_GetWindowSizeInPixels(window, &w, &h);

            glViewport(0, 0, w, h);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            SDL_GL_SwapWindow(window);
        }

    };
}