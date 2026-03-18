#pragma once

#include <cstdint>

#include "../../shared/bit-operations/bit_array.h"
#include "../../shared/bit-operations/bit_commands.h"

/* Created by Gordie Novak on 3/15/26.
 * Purpose: 
 */

namespace gobin {

    struct Motor {
        int32_t vel = 0;
        int32_t pos = 0;
        uint16_t model_id = 0;
        uint8_t id =0;
        bool torque = true;

        static Motor from_bits(uint8_t* data) {
            return Motor{
                .vel = *reinterpret_cast<int32_t*>(data),
                .pos = *reinterpret_cast<int32_t*>(data + sizeof(int32_t)),
                .model_id = *reinterpret_cast<uint8_t*>(data + offsetof(Motor, model_id)),
                .id = *reinterpret_cast<uint8_t*>(data + offsetof(Motor, id)),
                .torque = *reinterpret_cast<bool*>(data + offsetof(Motor, torque)),
            };
        }

    };
}
