#pragma once

#include <cstdint>

#include "../the-goblin/src/command/packets.h"
#include "../../shared/bit-operations/bit_array.h"

/* Created by Gordie Novak on 3/15/26.
 * Purpose: 
 */

namespace gobin {

    struct Motor {
        int32_t vel = 0;
        int32_t pos = 0;
        uint16_t model_num = 0;
        uint8_t id = 0;
        MotorModePacket mode = NO_MOTOR_MODE_P;
        bool torque = true;

    };
}
