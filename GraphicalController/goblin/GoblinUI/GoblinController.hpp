#pragma once


#include <imgui.h>

#include "../Goblin-Core/GoblinBrain.hpp"
#include "imgui-integration/InfoPanel.hpp"
#include "Motors/MotorSelector.hpp"
#include "Serializer/SerializeInterface.hpp"
#include "SDL_API/window/Window.hpp"

/* Created by Gordie Novak on 3/15/26.
 * Purpose: 
 */

namespace gobin {
    struct GoblinController : gan::InfoPanel {

        GoblinBrain& core;

        SerializeInterface<GoblinBrain::buf_size> serializer_ui;
        MotorSelector motor_ui;

        explicit GoblinController(const char windowName[], GoblinBrain& core)
            : InfoPanel("Gobin"),
                core(core) { }


        void display() {

            if (beginInfoPanel("Goblin Control Panel", {260, 400}))
                render_buttons();

            endInfoPanel();
        }


    private:

        void render_buttons() {


            gan::fstring<300> str_serial;
            str_serial.resize<300>();
            if (serializer_ui.button(str_serial, port.c_str())) {
                ImGui::SetKeyboardFocusHere(-1);
                size_t len = strlen(str_serial.data());
                serializer_ui.last_byte_count = len;
                core.send_serial((uint8_t*)str_serial.data(), len);
            }
            ImGui::Separator();

            // Velocity Slider
            //motor_ui.button(core.motors, core.max_motors);

            // Checkbox
            //ImGui::Checkbox("Enable/Disable Torque", &core.torque);

            // Allows us to
            ImGui::Text("sdl window pos: %i, %i", (int)window.getPosition().x, (int)window.getPosition().y);

        }



    };
}
