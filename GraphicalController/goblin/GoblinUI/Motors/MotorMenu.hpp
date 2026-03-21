#pragma once

#include <vector>
#include <gobdef.h>

#include "Goblin-Core/GoblinBrain.hpp"
#include "../the-goblin/src/goblin/Invariants.h"
#include "../Style/GobUI_Colors.h"
#include "GoblinUI/Style/GobUI_Style.h"

/* Created by Gordie Novak on 3/19/26.
 * Purpose: 
 */

namespace gobin {

    struct MotorMenu {
        i64 selection = -2;

        void update(GoblinBrain& core) {

            ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);

            static bool torque = true;
            std::string t_Str = std::string("Torque: ") + ((torque) ? "Enabled" : "Disabled");
            if (ImGui::Checkbox(t_Str.c_str(), &torque)) {
                for (auto& m : core.motors) {
                    m.torque = torque;
                    m.flagForUpdate();
                }
            }

            if (ImGui::CollapsingHeader("Motors")) {

                ImGui::PushID("motselect");
                u32 last_selection = selection;
                // loop through each button
                for (auto& m : core.motors) {
                    if (selection == -2) selection = 0;
                    ImGui::SetNextItemWidth(50);
                    if (last_selection == m.index) {
                        g_style.pushSelectedButton();
                        if (ImGui::Button(motorString(core.getMotorCore(m.index).motor_id).c_str())) {
                            selection = -1;
                        }
                        g_style.popSelectedButton();
                    } else {
                        if (ImGui::Button(motorString(core.getMotorCore(m.index).motor_id).c_str()))
                            selection = m.index;
                    }
                    ImGui::SameLine();
                }
                ImGui::PopID();
                ImGui::NewLine();

                if (selection >= 0 && selection < core.motors.size()) {
                    motorInst(core, core.motors[selection]);
                }
            }

        }

    private:
        std::string motorString(u8 motor_id) {
            return std::string("M") + std::to_string(motor_id);
        }

        void motorInst(GoblinBrain& core, MotorInterface& inst) {

            auto& m = core.getMotorCore(inst.index);

            /* —————————————————————— */
            ImGui::Separator();

            std::string m_id = "M";
            m_id += std::to_string(m.motor_id) + "{" + std::to_string(m.model_num) + "}"
                     + ": " +
                ((m.mode == JOINT_MODE) ? "Joint" : (m.mode == WHEEL_MODE) ? "Wheel" : "No Mode");

            ImGui::Text("%s", m_id.c_str());
            ImGui::Separator();

            ImGui::PushID((std::to_string(inst.index) + "mv").c_str());

            /* ------- Position & Velocity Sliders -------- */

            if (m.mode == JOINT_MODE) {
                ImGui::Text("Position:");
                ImGui::SliderInt("##m", &inst.pos, 0, 4094);

                if (inst.pos != m.pos) {
                    inst.flagForUpdate();
                }
            } else if (m.mode == WHEEL_MODE) {
                ImGui::Text("Velocity: %i", inst.vel);
                if (m.model_num == 1060)
                    ImGui::SliderInt("##m", &inst.vel, 0, 300);
                else
                    ImGui::SliderInt("##m", &inst.vel, invar::vel_min, invar::vel_max);

                if (inst.vel != m.vel) {
                    inst.flagForUpdate();
                }
            }

            // Now we display the radians
            ImGui::Text("Radians: %f", m.angle);

            /* ------- Joint/Wheel mode -------- */

            ImGui::SetNextItemWidth(100);

            GOB_GUI_BUTTON_DISABLE(
            if (ImGui::Button("Joint Mode")) {
                inst.mode = JOINT_MODE;
                inst.flagForUpdate();
            }, m.mode == JOINT_MODE, 0);

            ImGui::SetNextItemWidth(100);
            ImGui::SameLine();

            GOB_GUI_BUTTON_DISABLE(
            if (ImGui::Button("Wheel Mode")) {
                inst.mode = WHEEL_MODE;
                inst.flagForUpdate();
            }, m.mode == WHEEL_MODE, 1);

            /* —————————————————————— */
            ImGui::Separator();

            /* ------- Torque Toggle -------- */

            if (ImGui::Checkbox("Torque", &inst.torque)) {
                if (inst.torque != m.torque) {
                    inst.flagForUpdate();
                }
            }

            /* ------- MotorID Slider -------- */
            ImGui::Text("Motor ID Reassignment: ");
            ImGui::SliderInt("##msld", &inst.motor_id, 1, invar::max_motor_id - 1);

            bool prevent_reassign = false;
            for (u16 i = 0; i < core.motors.size(); i++) {
                if (core.getMotorCore(i).motor_id == inst.motor_id) {
                    prevent_reassign = true;
                    break;
                }
            }


            GOB_GUI_BUTTON_DISABLE(
            if (ImGui::Button("Reassign")) {
                if (inst.motor_id != m.motor_id) {
                    core.setMotorID(inst.index, inst.motor_id);
                }
            }, prevent_reassign, 3)

            /* —————————————————————— */
            ImGui::Separator();

            ImGui::PopID();
        }

    };

}
