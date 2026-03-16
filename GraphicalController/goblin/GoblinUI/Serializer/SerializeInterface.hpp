#pragma once
#include <cstdint>
#include <imgui.h>

#include "SDL_API/types/fstring.hpp"

/* Created by Gordie Novak on 3/15/26.
 * Purpose: 
 */

namespace goblin {

    template<size_t buf_size>
    struct SerializeInterface {

        uint32_t last_byte_count = 0;
        gan::fstring<buf_size> last_serial;

        bool button(gan::fstring<buf_size>& serial, const char port[] = "") {
            // Serialization
            gan::fstring<50> writing("serial > ::");
            writing << port;

            ImGui::Text(writing.c_str());
            const bool new_line = ImGui::InputText("##serialize_button",
                serial.data(), serial.length(), ImGuiInputTextFlags_EnterReturnsTrue);

            if (last_byte_count == 0) {
                ImGui::Text("> ...");
            } else {
                ImGui::Text("> '%s', [%i] bytes\n", last_serial.data(), last_byte_count);
            }

            if (new_line) {
                last_serial = serial.data();
                return true;
            }
            return false;
        }
    };

}
