#pragma once
#include <cstdint>
#include <imgui.h>
#include <string>

#include <Goblin-Core/Motor.hpp>
#include "MotorVelocitySliders.hpp"
#include "types/fstring.hpp"
#include "shared/Invariants.h"

/* Created by Gordie Novak on 3/15/26.
 * Purpose: 
 */

namespace goblin {

    struct MotorSelector {
        bool all = true;    //< if all motors are enabled.
        int8_t m_index = 0; //< index of the currently select motor

        MotorVelocitySlider velocity_ui;

        void button(Motor motors[], uint8_t num_motors) {

            ImGui::Text("Motor Controller");

            // the "all button"
            if (all) {
                color_button();
                if (ImGui::Button("All")) {
                    all = true;
                }
                pop_colors();
            } else {
                if (ImGui::Button("All")) {
                    all = true;
                }
            }

            for (int i = 0; i < num_motors; i++) {
                ImGui::SameLine();
                if (!all && i == m_index) {
                    color_button();
                    if (ImGui::Button(std::to_string(i).c_str())) {
                        m_index = i;
                        all = false;
                    }
                    pop_colors();
                } else {
                    if (ImGui::Button(std::to_string(i).c_str())) {
                        m_index = i;
                        all = false;
                    }
                }
            }

            std::string selection = "motor: ";
            if (all)
                selection += "{All}";
            else
                selection += '{' + std::to_string(m_index) + '}';
            ImGui::Text("%s", selection.c_str());

            if (all == true) {
                int32_t vel = motors[m_index].vel;
                velocity_ui.button(vel);
                for (int i = 0; i < num_motors; i++) {
                    motors[i].vel = vel;
                }
            } else {
                velocity_ui.button(motors[m_index].vel);
            }

            zero_out_button(motors, num_motors);

            display_motor_stats(motors, num_motors);
        }

    private:
        void zero_out_button(Motor motors[], uint8_t num_motors) {
            ImGui::BeginGroup();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("~{");
            ImGui::SameLine();
            if (ImGui::Button("Zero Out")) {
                if (all) {
                    for (int i = 0; i < num_motors; i++) {
                        motors[i].vel = 0;
                    }
                } else {
                    motors[m_index].vel = 0;
                }
                velocity_ui.acceleration = 0.f;
            }
            ImGui::SameLine();
            ImGui::Text("}~ ");
            ImGui::EndGroup();
        }

        static void display_motor_stats(Motor motors[], uint8_t num_motors) {

            ImGui::BeginTable("Motor Info", 2, ImGuiTableFlags_BordersInnerV);

            ImGui::TableSetupColumn("Motor");
            ImGui::TableSetupColumn("Status");
            ImGui::TableHeadersRow();

            gan::fstring<30> motor_info = "";
            gan::fstring<30> motor_status = "";
            for (int i = 0; i < num_motors; i++ ) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                motor_info << motors[i].id << ": " << motors[i].model_id;
                ImGui::Text("%s", motor_info.c_str());
                ImGui::TableSetColumnIndex(1);
                motor_status << "v: " << motors[i].vel << " p: ";// << motors[i].pos;
                ImGui::Text("%s", motor_status.c_str());

                motor_info.clear();
                motor_status.clear();
            }

            ImGui::EndTable();
        }

        static void color_button() {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.0f, 1.0f));         // Normal color (dark green)
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.7f, 0.0f, 1.0f));   // Hovered color (brighter green)
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.9f, 0.0f, 1.0f));
        }

        static void pop_colors() {
            ImGui::PopStyleColor(3);
        }

    };


}
