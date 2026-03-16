#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <imgui.h>
#include "shared/Invariants.h"

/* Created by Gordie Novak on 3/15/26.
 * Purpose: 
 */

namespace goblin {

    struct MotorVelocitySlider {
        float acceleration = 0.f;

        void button(int32_t& velocity) {
            ImGui::SetNextItemWidth(90.f);
            ImGui::SliderInt("vel .. acc", &velocity, invar::vel_min, invar::vel_max);

            ImGui::SetNextItemWidth(80.f);
            ImGui::SameLine();
            acceleration /= 1.2f;
            if (fabsf(acceleration) < 0.05)
                acceleration = 0.f;
            ImGui::SliderFloat("## acceler_a", &acceleration, -6.0f, 6.0f);
            velocity += acceleration;
            velocity = std::clamp(velocity, invar::vel_min, invar::vel_max);



        }
    };

}
