#pragma once
#include "Workbench.h"
#include "../core/crstream.h"

namespace gobin {

    class Goblin;

    enum MotorMode : u8 {
        NO_MOTOR_MODE = 0,
        WHEEL_MODE = 1,
        JOINT_MODE = 2
    };

    struct Motor {
        u16 model_num = 0;  //< model number of the motor.
        u8 id = -1;         //< id of the motor.
        MotorMode mode = NO_MOTOR_MODE; //< current mode of the moder.

    /* ----------------------------------------------------- */
    /* --------------------- Velocity ---------------------- */

        /// Sets the velocity of the motor.
        void setVelocity(i32 velocity) const {
            if (mode != WHEEL_MODE) {
                CRError(VELOCITY_SET_NOT_WHEEL_MODE, id);
                return;
            } if (velocity > invar::vel_max) {
                CRError(VELOCITY_SET_ABOVE_MAX, id);
                velocity = invar::vel_max;
            } else if (velocity < -invar::vel_max) {
                CRError(VELOCITY_SET_BELOW_MIN, id);
                velocity = -invar::vel_max;
            }

            if (!workbench::wb.goalVelocity(id, velocity)) {
                CRError(WHEEL_MODE_SET_FAIL, id);
            }
        }

        void changeVelocity(i32 d_velocity) const {
            i32 vel = getVelocity() + d_velocity;
            if (vel > invar::vel_max) {
                vel = invar::vel_max;
            } else if (vel < invar::vel_min) {
                vel = invar::vel_min;
            }
            setVelocity(vel + d_velocity);
        }

        [[nodiscard]] i32 getVelocity() const {
            i32 vel{};
            if (!workbench::wb.getPresentVelocityData(id, &vel)) {
                CRError(VELOCITY_GET_FAIL, id);
            }
            return vel;
        }

    /* ----------------------------------------------------- */
    /* --------------------- Position ---------------------- */

        void changePosition(i32 position) const {
            const i32 pos = getPosition();
            setPosition(pos + position);
        }

        /// Sets position. Max position is @code invar::max_pos@endcode. Minimum is @code invar::min_pos@endcode.
        void setPosition(i32 position) const {
            if (mode != JOINT_MODE) {
                CRError(POSITION_SET_NOT_JOINT_MODE, id);
                return;
            } if (position > invar::pos_max) {
                CRError(POSITION_SET_ABOVE_MAX, id);
                position = invar::pos_max;
            } else if (position < invar::pos_min) {
                CRError(POSITION_SET_BELOW_MIN, id);
                position = invar::pos_min;
            }

            if (!workbench::wb.goalPosition(id, position)) {
                CRError(POSITION_SET_FAIL, id);
            }
        }

        [[nodiscard]] i32 getPosition() const {
            i32 pos;
            if (!workbench::wb.getPresentPositionData(id, &pos)) {
                CRError(POSITION_GET_FAIL, id);
            }
            return pos;
        }

    /* ----------------------------------------------------- */
    /* --------------------- Radians ---------------------- */

        [[nodiscard]] float getRadians() const {
            float radians;
            if (!workbench::wb.getRadian(id, &radians, &workbench::cr_log)) {
                CRError(RADIANS_GET_FAIL, id);
                return 0.f;
            }
            return radians;
        }

    /* ----------------------------------------------------- */
    /* --------------------- Modes ------------------------- */

        void setJointMode(const i32 vel, const i32 acc) {
            if (!workbench::wb.jointMode(id, vel, acc)) {
                CRError(JOINT_MODE_SET_FAIL, id);
            } else {
                CRPrint("Setting joint mode");
                mode = JOINT_MODE;
            }
        }

        void setWheelMode(const i32 vel) {
            if (!workbench::wb.wheelMode(id, vel)) {
                CRError(WHEEL_MODE_SET_FAIL, id);
            } else {
                CRPrint("Setting wheel mode.");
                mode = WHEEL_MODE;
            }
        }

    /* ----------------------------------------------------- */
    /* --------------------- Misc  ------------------------- */

        void setTorque(b8 torque) const {
            if (torque) {
                enableTorque();
            } else {
                disableTorque();
            }
        }

        void enableTorque() const {
            if (!workbench::wb.torqueOn(id)) {
                CRError(TORQUE_ENABLE_FAIL, id);
            }
        }

        void disableTorque() const {
            if (!workbench::wb.torqueOff(id)) {
                CRError(TORQUE_DISABLE_FAIL, id);
            }
        }

        Motor() = default;

    private:
        void setMotorID(const u8 motor_id) {
            if (!workbench::wb.changeID(id, motor_id)) {
                CRError(MOTOR_ID_SET_FAIL, id);
            } else {
                id = motor_id;
            }
        }

        friend class MotorList;

    };

}
