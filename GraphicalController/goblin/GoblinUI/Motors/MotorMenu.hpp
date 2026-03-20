#pragma once


#include <vector>
#include <gobdef.h>

#include "Goblin-Core/GoblinBrain.hpp"
#include "GoblinUI/GobUpdate.hpp"
#include "shared/Invariants.h"

/* Created by Gordie Novak on 3/19/26.
 * Purpose: 
 */

namespace gobin {

    struct MotorInst {
        i32 vel;
        i32 pos;
        u32 model_id;
        u8 id;
        u8 index;
        MotorModePacket mode;
    };

    struct MotorMenu {
        // All the velocities of the motor menu
        std::vector<MotorInst> mot;

        void update(GoblinBrain& core, GobUpdate& update) {

            if (core.motors.size() != mot.size()) {
                mot.resize(core.motors.size());
                for (size_t i = 0; i < mot.size(); ++i) {
                    mot[i].vel = core.motors[i].vel;
                    mot[i].pos = core.motors[i].pos;
                    mot[i].mode = core.motors[i].mode;
                    mot[i].id = core.motors[i].id;
                    mot[i].index = i;
                    mot[i].model_id = core.motors[i].model_num;
                }
            }


            if (update.update1000 && mot.empty()) {
                //core.queryMotorList();
                return;
            }


            if (ImGui::CollapsingHeader("Motors")) {
                for (auto& m : mot) {
                    motorInst(core, m, update.update100);
                }
            }

        }

    private:
        void motorInst(GoblinBrain& core, MotorInst& inst, bool update) {
            ImGui::Separator();
            std::string m_id = "M";
            m_id += std::to_string(inst.id);
            m_id += " {" + std::to_string(inst.model_id) + "}";

            std::string motor_title = m_id + ": " +
                ((inst.mode == JOINT_MODE_P) ? "Joint" : (inst.mode == WHEEL_MODE_P) ? "Wheel" : "No Mode");

            ImGui::Text("%s", motor_title.c_str());
            ImGui::Separator();

            ImGui::PushID(m_id.c_str());

            if (inst.mode == JOINT_MODE_P) {
                ImGui::Text("Position:");
                ImGui::SliderInt("##motor", &inst.pos, -300, 300);
                ImGui::Text("Velocity: %i", inst.vel);

                if (update && inst.pos != core.motors[inst.index].pos) {
                    core.setPosition(inst.index, inst.pos);
                }
            } else if (inst.mode == WHEEL_MODE_P) {
                ImGui::Text("Position: %i", inst.pos);
                ImGui::Text("Velocity: ", inst.vel);
                if (inst.model_id == 1060)
                    ImGui::SliderInt("##motor", &inst.vel, 0, 300);
                else
                    ImGui::SliderInt("##motor", &inst.vel, invar::vel_min, invar::vel_max);

                if (update && inst.vel != core.motors[inst.index].vel) {
                    core.setVelocity(inst.index, inst.vel);
                }
            }
            ImGui::SetNextItemWidth(100);

            bool b = inst.mode == JOINT_MODE_P;
            if (b) ImGui::BeginDisabled();
            if (ImGui::Button("Joint Mode")) {
                core.setJointMode(inst.index);
                inst.mode = JOINT_MODE_P;
            }
            ImGui::SetNextItemWidth(100);
            ImGui::SameLine();

            if (b) ImGui::EndDisabled();
            b = inst.mode == WHEEL_MODE_P;
            if (b) ImGui::BeginDisabled();
            if (ImGui::Button("Wheel Mode")) {
                core.setWheelMode(inst.index);
                inst.mode = WHEEL_MODE_P;
            }
            if (b) ImGui::EndDisabled();
            ImGui::Separator();

            ImGui::PopID();
        }

    };

}
