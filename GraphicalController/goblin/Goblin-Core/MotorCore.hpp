#pragma once

#include <cstdint>
#include <goblin/gobdef.h>

#include "../the-goblin/src/command/packets.h"

/* Created by Gordie Novak on 3/15/26.
 * Purpose: 
 */

namespace gobin {

    enum MotorCoreMode {
        NO_MOTOR_MODE,
        JOINT_MODE,
        WHEEL_MODE
    };

    /// Record type for a Motor.
    struct MotorCore {
        int32_t vel = 0;
        int32_t pos = 0;
        float angle = 0;
        uint16_t model_num = 0;
        uint8_t motor_id = 0;
        MotorCoreMode mode = NO_MOTOR_MODE;
        bool torque = true;
    };

    /// This is the motor that external objects interact with.
    /// They edit these properties & the goblin brain automatically updates.
    struct MotorInterface {
        union { i32 vel{}, pos; };
        const u32 index   = 0;
        float radians     = 0; ///< writing to this does nothing.
        i32 motor_id      = 0;
        bool torque       = false;
        MotorCoreMode mode = NO_MOTOR_MODE;

        MotorInterface(const MotorCore& core, u32 index, bool update_vel_pos = true)
            : radians(core.angle), index(index), torque(core.torque), mode(core.mode),
                motor_id(core.motor_id), needs_updating(false)
        {
            if (update_vel_pos) {
                if (mode == WHEEL_MODE) {
                    vel = core.vel;
                } else if (mode == JOINT_MODE) {
                    pos = core.pos;
                } else vel = 0;
            }
        }

        void flagForUpdate() {
            needs_updating = true;
        }

    private:
        friend class GoblinCore;
        bool needs_updating = false;
    };
}
