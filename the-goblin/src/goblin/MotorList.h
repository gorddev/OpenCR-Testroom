#pragma once

#include "Motor.h"
#include "../command/packets.h"
#include <DynamixelWorkbench.h>

namespace gobin {
    constexpr float motor_pi = 3.14159;

    int32_t radian_to_pos(float radian) {
        while (radian < 0) {
            radian += 2*motor_pi;
        }
        return invar::pos_max * static_cast<i32>(radian/(2*motor_pi));
    }

    /// Manages finding, storing, verifying, and reassigning motors and their ids.
    class MotorList {
    private:
        gobin::Motor motorArr[invar::max_motor_id];
    public:
        u8 motor_count{};

        void addMotor(i8 motor_id, u16 model_num) {
            if (motor_count < invar::max_motor_id) {
                motorArr[motor_count].id = motor_id;
                motorArr[motor_count].model_num = model_num;
                motorArr[motor_count].setJointMode(invar::vel_max, invar::acc_rate);
                //motorArr[motor_count].setPosition(radian_to_pos(motorArr[motor_count].getRadians()));
                motor_count++;

            }
        }

        void sendSerializedMotors(const bool reset) {
            if (true) {

                MotorPacket motors[invar::max_motor_id];

                u8 count = 0;
                for (auto& m: motorArr) {
                    if (count >= motor_count)
                        break;
                    motors[count].id = m.id;
                    motors[count].mode = static_cast<MotorModePacket>(m.mode);
                    motors[count].model_num = m.model_num;
                    if (m.mode == WHEEL_MODE)
                        motors[count].vel = m.getVelocity();
                    else
                        motors[count].pos = m.getPosition();
                    motors[count].radians = m.getRadians();
                    count++;
                }

                if (reset)
                    CRCommand({COM_INFO, T_MOTOR_LIST, motor_count}, &motors, sizeof(MotorPacket)*count);
                else
                    CRCommand({COM_STATUS, T_MOTOR_LIST, motor_count}, &motors, sizeof(MotorPacket)*count);
            }
        }

        b8 findMotors() {
            if (motor_count != 0) {
                CRError(SCANNED_FOR_MOTORS_MORE_THAN_ONCE, motor_count);
            }

            u16 model_num;
            motor_count = 0;

            for (i8 id = 0; id < invar::max_motor_id; id++) {
                if (workbench::wb.ping(id, &model_num, &workbench::cr_log)) {
                    // if we find the motor, add it to our list of found motors.
                    addMotor(id, model_num);

                    workbench::wb.itemRead(4, nullptr, nullptr);
                }
            }

            if (motor_count == 0) {
                CRError(NO_MOTORS_FOUND_IN_SCAN, invar::max_motor_id);
            }

            // calls sendSerializedMotors
            sendSerializedMotors(true);

            return true;
        }

        [[nodiscard]] b8 verifyMotor(const u8 motor_id) const {
            for (u8 i = 0; i < motor_count; i++) {
                if (motorArr[i].id == motor_id) {
                    return true;
                }
            }
            return false;
        }

        [[nodiscard]] Motor* at(u8 motor_id) {
            for (u8 i = 0; i < motor_count; i++) {
                if (motorArr[i].id == motor_id) {
                    return &motorArr[i];
                }
            }
            CRError(MOTOR_DOES_NOT_EXIST, motor_id);
            return nullptr;
        }

        /// WARNING: Has Potential to Exit.
        [[nodiscard]] Motor& operator[](u8 motor_id) {
            Motor* m = at(motor_id);
            if (!m) CRPanic("MotorList::operator[]. Motor does not exist.");
            return *m;
        }

        void changeMotorID(const u8 motor_id, const u8 new_motor_id) {
            if (new_motor_id > invar::max_motor_id) {
                CRError(INVALID_NEW_MOTOR_ID_ABOVE_LIMIT, new_motor_id);
                return;
            }
            Motor* motor = nullptr;
            for (u8 i = 0; i < motor_count; i++) {
                if (motorArr[i].id == motor_id) {
                    motor = &motorArr[i];
                } else if (motorArr[i].id == new_motor_id) {
                    CRError(INVALID_NEW_MOTOR_ID_MOTOR_ALREADY_HAS_ID, new_motor_id);
                    return;
                }
            }
            if (!motor) {
                CRError(INVALID_NEW_MOTOR_ID_MOTOR_DOES_NOT_EXIST, motor_id);
                return;
            }

            motor->setMotorID(new_motor_id);

            motor_count = 0;
            sendSerializedMotors(true);

        }

        class iterator {
            Motor* m;
        public:
            explicit iterator(Motor* m) : m(m) {}

            iterator& operator++() {++m; return *this;}
            Motor& operator*() const {return *m;}
            Motor* operator->() const {return m;}
            b8 operator==(const iterator& other) const {return m == other.m;}
            b8 operator!=(const iterator& other) const {return m != other.m;}
        };

        iterator begin() {return iterator{motorArr};}
        iterator end() {return iterator{motorArr + motor_count};}

        friend class Goblin;
        MotorList() = default;
    };
}
