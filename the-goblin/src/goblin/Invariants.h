#pragma once

#include "../core/crdef.h"

/* Created by Gordie Novak on 3/15/26.
 * Purpose:
 */

namespace invar {
    /* ------------------- */
    constexpr i32 vel_max = 330;
    constexpr i32 vel_min = -vel_max;
    /* ------------------- */
    constexpr i32 pos_max = 4095;
    constexpr i32 pos_min = -pos_max;
    /* ------------------- */
    constexpr int max_motor_id = 12;
    /* ------------------- */
    constexpr i32 acc_rate = 100;           ///< Acceleration rate.
    constexpr i32 vel_rate_joint = vel_max; ///< Velocity during joint mode.
}
