#pragma once


#include <imgui.h>

#include "../Goblin-Core/GoblinCore.hpp"
#include "imgui-integration/InfoPanel.hpp"
#include "Motors/MotorSelector.hpp"
#include "Serializer/SerializeInterface.hpp"
#include "SDL_API/window/Window.hpp"

/* Created by Gordie Novak on 3/15/26.
 * Purpose: 
 */

namespace goblin {
    struct GoblinController : gan::InfoPanel {

        GoblinCore& core;

        SerializeInterface<GoblinCore::buf_size> serializer_ui;
        MotorSelector motor_ui;

        explicit GoblinController(const char windowName[], GoblinCore& core)
            : InfoPanel("Gobin"),
                core(core) { }


        void display() {

            if (beginInfoPanel("Goblin Control Panel", {260, 400}))
                render_buttons();

            endInfoPanel();
        }


    private:

        void render_buttons() {


            if (serializer_ui.button(core.serial, GoblinCore::port)) {
                ImGui::SetKeyboardFocusHere(-1);
                serializer_ui.last_byte_count = core.send_serial();
            }
            ImGui::Separator();

            // Velocity Slider
            motor_ui.button(core.motors, core.num_motors);

            // Checkbox
            ImGui::Checkbox("Enable/Disable Torque", &core.torque);

            // Allows us to
            ImGui::Text("sdl window pos: %i, %i", (int)window.getPosition().x, (int)window.getPosition().y);

        }



    };
}
