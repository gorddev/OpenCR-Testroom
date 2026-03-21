#pragma once

#include "../Goblin-Core/GoblinBrain.hpp"
#include "SDL_API/imgui-integration/InfoPanel.hpp"
#include "Motors/MotorMenu.hpp"
#include "Serializer/SerializeInterface.hpp"
#include "SDL_API/window/Window.hpp"
#include "GoblinConnectUI.hpp"
#include "GoblinConsoleUI.hpp"
#include "Style/GobUI_Style.h"

/* Created by Gordie Novak on 3/15/26.
 * Purpose: 
 */

namespace gobin {



    struct GoblinGUI : gan::InfoPanel {

        GoblinBrain& core;
        GobUI_Style style;

        GoblinConnectUI connectUI;
        MotorMenu motor_menu;


        explicit GoblinGUI(const char windowName[], GoblinBrain& core)
            : InfoPanel(windowName, gan::WindowFloatOnTop | gan::WindowTransparent | gan::WindowResizable),
                core(core), connectUI(core.brain_log) {

            g_style.body_font = imGuiIO.Fonts->AddFontFromFileTTF(
                (gan::files::assets()/"gob_body_font.ttf").c_str(),
                22.f);

            g_style.header_font = imGuiIO.Fonts->AddFontFromFileTTF(
                (gan::files::assets()/"gob_header_font.ttf").c_str(),
                26.f);

            g_style.input_font = imGuiIO.Fonts->AddFontFromFileTTF(
                (gan::files::assets()/"gob_input_font.ttf").c_str(),
                18.f);

            ImFontConfig config;
            config.OversampleH = 3; // Increases horizontal resolution
            config.OversampleV = 1;

            g_style.console_font = imGuiIO.Fonts->AddFontFromFileTTF(
                (gan::files::assets()/"gob_console_font.ttf").c_str(),
                14.f, &config);



            /*
            imGuiIO.Fonts->AddFontDefault();

            ImFontConfig config;
            config.SizePixels = roundf(13.0f * scale_factor);
            imGuiIO.Fonts->AddFontDefault(&config);
            */

        }


        void display() {
            g_style.pushGobDefault();
            beginInfoPanel();

            g_style.pushHeaderFont();

            bool b = ImGui::Begin("Goblin Control Panel", nullptr,
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoScrollbar
                |ImGuiWindowFlags_AlwaysAutoResize
            );

            g_style.popHeaderFont();
            if (b) {
                render_buttons();
            }


            g_style.popGobDefault();

            ImGui::End();

            endInfoPanel();
        }
    private:

        void render_buttons() {

            ImGui::Separator();

            connectUI.button(core);

            ImGui::BeginGroup();
            GoblinConsoleUI::displayConsole("Local Console", core.brain_log);
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            GoblinConsoleUI::displayConsole("CR Console", core.open_cr_log);
            ImGui::EndGroup();

            if (core.is_connected()) {
                // Velocity Slider
                motor_menu.update(core);
            }
            // Checkbox
            //ImGui::Checkbox("Enable/Disable Torque", &core.torque);
            ImGui::Text("%s", port_record.c_str());
            // Allows us to
            ImGui::Text("sdl window pos: %i, %i", (int)window.getPosition().x, (int)window.getPosition().y);
        }



    };
}
