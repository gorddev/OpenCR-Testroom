#pragma once

#include "../Goblin-Core/GoblinBrain.hpp"
#include "imgui-integration/InfoPanel.hpp"
#include "Motors/MotorMenu.hpp"
#include "Motors/MotorSelector.hpp"
#include "Serializer/SerializeInterface.hpp"
#include "SDL_API/window/Window.hpp"

/* Created by Gordie Novak on 3/15/26.
 * Purpose: 
 */

namespace gobin {



    struct GoblinController : gan::InfoPanel {

        GoblinBrain& core;

        SerializeInterface<300> serializer_ui;
        MotorSelector motor_ui;
        MotorMenu motor_menu;

        explicit GoblinController(const char windowName[], GoblinBrain& core)
            : InfoPanel(windowName, gan::WindowFloatOnTop | gan::WindowTransparent | gan::WindowResizable),
                core(core) { }


        void display() {
            pushGoblinStyle();
            if (beginInfoPanel("Goblin Control Panel", {260, 400}))
                render_buttons();
            popGoblinStyle();
            endInfoPanel();
        }
    private:

        void render_buttons() {

            gan::fstring<300> str_serial;
            str_serial.resize<300>();
            if (serializer_ui.button(str_serial, core.port.get_path().c_str())) {
                ImGui::SetKeyboardFocusHere(-1);
                size_t len = strlen(str_serial.data());
                serializer_ui.last_byte_count = len;
                core.port.write((uint8_t*)str_serial.data(), len);
            }
            ImGui::Separator();

            // Velocity Slider
            motor_menu.update(core);

            // Checkbox
            //ImGui::Checkbox("Enable/Disable Torque", &core.torque);
            ImGui::Text("%s", port_record.c_str());
            // Allows us to
            ImGui::Text("sdl window pos: %i, %i", (int)window.getPosition().x, (int)window.getPosition().y);
        }



    };
}
