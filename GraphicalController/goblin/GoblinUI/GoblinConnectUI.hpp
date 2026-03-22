#pragma once

#include <string>

#include "gobdef.h"
#include "Goblin-Core/Verify_Goblin.hpp"
#include "types/fstring.hpp"

namespace gobin {

    struct GoblinConnectUI {

        gan::fstring<100> portPath;       /// Path to the port the user wants to choose.
        u64 baudrate;    /// String that represents the baudrate the user wants to represent.

        explicit GoblinConnectUI(GoblinLog& log) {
            u64 baud;
            std::string path;
            if (data::getConfigData(&path, &baud, log)) {
                portPath = path.c_str();
                baudrate = baud;
            } else {
                portPath.resize<100>();
                baudrate = 1000000;
            }
        }

        void button(GoblinCore& core) {

            if (!core.is_connected()) {
                try_to_connect(core);
            } else {
                try_disconnect(core);
            }

        }

        void try_to_connect(GoblinCore& core) {
            ImGui::Separator();
            ImGui::Text("Connect to Port: ");
            g_style.pushInputFont();
            ImGui::InputText("##ConnectGoblin", portPath.data(), 100);
            g_style.popInputFont();

            ImGui::SetNextItemWidth(80);
            GOB_GUI_BUTTON_DISABLE(
            if (ImGui::Button("9600")) { baudrate = 9600;},
            baudrate == 9600, 1);

            ImGui::SameLine();
            ImGui::SetNextItemWidth(80);
            GOB_GUI_BUTTON_DISABLE(
            if (ImGui::Button("115200")) { baudrate = 115200;},
            baudrate == 115200, 2);

            ImGui::SameLine();
            ImGui::SetNextItemWidth(80);
            GOB_GUI_BUTTON_DISABLE(
            if (ImGui::Button("1000000")) { baudrate = 1000000;},
            baudrate == 1000000, 3);

            ImGui::Text("Current Baudrate: %llu", baudrate);

            ImGui::SetNextItemWidth(400);
            static bool connect_failed = false;
            if (ImGui::Button("Try Connect")) {
                if (!verify::isPort(portPath.c_str(), core.brain_log)) {
                    connect_failed = true;
                } else {
                    connect_failed = !core.openPort(portPath.c_str(), baudrate);
                }
            } if (connect_failed) {
                ImGui::Text("Connection to port '%s' failed.", portPath.data());
            }
            ImGui::Separator();
        }

        void try_disconnect(GoblinCore& core) {
            ImGui::Separator();

            ImGui::Text("Connected to Port: ");
            ImGui::SameLine();
            g_style.pushInputFont();
            ImGui::Text("%s", portPath.data());
            g_style.popInputFont();


            if (ImGui::Button("Scan Motors")) {
                core.requestMotorScan();
            } ImGui::SameLine();
            if (ImGui::Button("Query Motors")) {
                core.queryMotors();
            } ImGui::SameLine();
            g_style.pushSpecialButton();
            if (ImGui::Button("Disconnect")) {
                core.closePort();
            } ImGui::SameLine();
            if (ImGui::Button("RESET")) {
                core.resetArduino();
            } g_style.popSpecialButton();
            ImGui::Separator();

        }
    };

}
