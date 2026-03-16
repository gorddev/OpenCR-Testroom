#pragma once

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

#include "SDL_API/window/Window.hpp"

/* Created by Gordie Novak on 3/16/26.
 * Purpose:
 */

namespace gan {
    class InfoPanel {
    protected:
        ImGuiContext* imgui_context;
        gan::Window window;
        SDL_Renderer* sdl_renderer;
        dim2 sizeReq;
        bool validFrame = false;

    public:
        explicit InfoPanel(const char windowName[], WindowProperty prop = WindowFloatOnTop) : imgui_context(ImGui::CreateContext()),
            window(windowName, {750, 500}, WindowTransparent | prop),
            sdl_renderer(SDL_CreateRenderer(window, "software"))
        {
            SDL_SetWindowResizable(window, false);
            SDL_SetRenderVSync(sdl_renderer, true);
            ImGui_ImplSDL3_InitForSDLRenderer(window, sdl_renderer);
            ImGui_ImplSDLRenderer3_Init(sdl_renderer);
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
            ImGui_ImplSDLRenderer3_Shutdown();
            ImGui::DestroyContext(imgui_context);
            SDL_DestroyRenderer(sdl_renderer);
            SDL_DestroyWindow(window);
        }

    protected:
        [[nodiscard]] bool beginInfoPanel(const char name[], dim2 size) {
            if (window.isHidden())
                return false;

            ImGui::SetCurrentContext(imgui_context);

            // Set up the new frames for each.
            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            setup_window();
            validFrame = ImGui::Begin(name, nullptr,
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoResize
            );

            if (!validFrame) {
                endInfoPanel();
            }

            return validFrame;

            // make sure our window size is correct.
        }

        void endInfoPanel() const {
            if (window.isHidden())
                return;
            // resize according to window size.
            resize_window(sizeReq);

            ImGui::End();

            draw();
        }

    private:
        void setup_window() const {
            ImGui::SetNextWindowPos({0,0}, ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(0,0), ImGuiCond_FirstUseEver);
        }

        void resize_window(const dim2& size) const {
            gan::dim2 w_dim = window.getDimensions();
            auto im_size = ImGui::GetWindowSize();
            if (std::abs((int)im_size.x -(int)w_dim.w) > 1 || std::abs((int)im_size.y - (int)w_dim.h) > 1) {
                window.setDimensions({(int)im_size.x, (int)im_size.y});
            }
        }


        void draw() const {
            ImGui::Render();
            SDL_RenderClear(sdl_renderer);
            ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), sdl_renderer);
            SDL_RenderPresent(sdl_renderer);
        }

    };
}