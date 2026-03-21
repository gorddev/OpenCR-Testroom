#pragma once

#include "gobdef.h"
#include "../Goblin-Core/GoblinLog.h"
#include "Style/GobUI_Style.h"
#include <string>

namespace gobin {

    struct GoblinConsoleUI {

        static void displayConsole(const std::string& console_name, GoblinLog& log) {
            g_style.pushInputFont();
            ImGui::Text("%s", console_name.c_str());
            g_style.popInputFont();

            ImGui::PushID(console_name.c_str());
            g_style.pushConsoleFont();
            ImGui::BeginChild("console", ImVec2((ImGui::GetContentRegionMax().x- 20)/2, 100), true);

            u8 count = 0;
            for (auto& l : log) {
                ImGui::TextWrapped("[%u]: %s", count++, l.c_str());
                ImGui::Separator();
            }
            ImGui::EndChild();
            g_style.popConsoleFont();
            ImGui::PopID();

        }

    };

}
