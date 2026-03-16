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

    public:
        explicit InfoPanel(const char windowName[]) : imgui_context(ImGui::CreateContext()),
            window(windowName, {500, 500}, gan::WindowTransparent),
            sdl_renderer(SDL_CreateRenderer(window, "software"))
        {
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
            ImGui::DestroyContext(imgui_context);
            SDL_DestroyRenderer(sdl_renderer);
        }

    protected:
        void beginInfoPanel(const char name[]) const {
            if (window.isHidden())
                return;

            ImGui::SetCurrentContext(imgui_context);

            // Set up the new frames for each.
            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            setup_window();

            ImGui::Begin(name, nullptr,
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoScrollbar |
                ImGuiTableFlags_SizingFixedFit
            );

            // make sure our window size is correct.
            setup_window();
        }

        void endInfoPanel() const {
            // resize according to window size.
            resize_window();

            ImGui::End();

            draw();
        }

    private:
        void setup_window() const {
            gan::dim2 w_dim = window.getDimensions();
            ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
            ImGui::SetNextWindowSize({(float)w_dim.w, 0.0f}, ImGuiCond_Always);
        }

        void resize_window() const {
            gan::dim2 w_dim = window.getDimensions();
            const float content_height = ImGui::GetCursorPosY() + ImGui::GetStyle().WindowPadding.y;
            const float content_width = ImGui::GetItemRectMax().x + ImGui::GetStyle().WindowPadding.x;

            if (std::abs(content_height - w_dim.h) > 1) {
                window.setDimensions({(int)content_width, (int)content_height});
            }
        }

        void new_frame() const {

        }

        void draw() const {
            ImGui::Render();
            SDL_RenderClear(sdl_renderer);
            ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), sdl_renderer);
            SDL_RenderPresent(sdl_renderer);
        }

    };
}