#pragma once

#include <cstdint>

/* Created by Gordie Novak on 3/15/26.
 * Purpose: 
 */

namespace goblin {

    struct Motor {
        int32_t vel = 0;
        int32_t pos = 0;
        uint16_t model_id = 0;
        uint8_t id =0;
        bool torque = true;
    };
}
