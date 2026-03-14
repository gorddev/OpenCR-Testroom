#pragma once
#include "Workbench.h"

namespace dxl {

    class Goblin;

    enum MotorMode : uint8_t{
        NO_MOTOR_MODE = 0,
        WHEEL_MODE = 1,
        JOINT_MODE = 2
    };

    struct Motor {
        uint16_t model_num = 0;
        uint8_t id = -1;
        MotorMode mode = NO_MOTOR_MODE;

        void setVelocity(float velocity) const {
            if (mode != WHEEL_MODE) {
                workbench::success("Warning: Cannot set velocity as motor is not in wheel mode.");
                return;
            }
            if (workbench::wb.goalVelocity(id, velocity, &workbench::cr_log)) {
                CR_PANIC("Failed to set velocity mode for motor ");
                CR_PRINT(id); CR_EXIT;
            }
        }

        void setPosition(int32_t position) const {
            if (mode == JOINT_MODE) {
                if (workbench::wb.goalPosition(id, position)) {
                    CR_PANIC("Failed to set position mode for motor ");
                    CR_PRINT(id); CR_EXIT;
                }
            } else {
                workbench::success("Warning: Cannot set position as motor is not in joint mode.");
            }
        }

        float getVelocity() const {
            float vel;
            workbench::wb.getVelocity(id, &vel);
            return vel;
        }

        int32_t getPosition() const {
            int32_t pos;
            workbench::wb.getPresentPositionData(id, &pos);
            return pos;
        }

        float getRadians() const {
            float radians;
            workbench::wb.getRadian(id, &radians);
            return radians;
        }

        void setJointMode(uint32_t vel, uint32_t acc) {
            if (!workbench::wb.jointMode(id, vel, acc)) {
                CR_PANIC("Failed to set join mode for motor ");
                CR_PRINT(id); CR_EXIT;
            }
            mode = JOINT_MODE;
        }

        void setWheelMode(uint32_t vel) {
            if (!workbench::wb.wheelMode(id, vel)) {
                CR_PANIC("Failed to set wheel mode for motor ");
                CR_PRINT(id); CR_EXIT;
            }
            mode = WHEEL_MODE;
        }


        Motor() = default;

    };

    struct MotorList {
    private:
        Motor motorArr[invar::max_motor_id];
    public:
        int8_t count = 0;

        void addMotor(int8_t motor_id, uint16_t model_num) {
            if (count < invar::max_motor_id) {
                motorArr[count].id = motor_id;
                motorArr[count].model_num = model_num;
                count++;
            }
        }

        void findMotors() {
            if (count != 0) {
                CR_PANIC("Already scanned for motors.");
                CR_EXIT;
            }

            uint16_t model_num;

            workbench::status("Checking motor_ids:");
            for (int8_t id = 0; id < invar::max_motor_id; id++) {
                Serial.print(id);
                if (workbench::wb.ping(id, &model_num, &workbench::cr_log)) {
                    // if we find the motor, add it to our list of found motors.
                    addMotor(id, model_num);
                    Serial.print("!");
                }
                Serial.print(" ");
            }
            Serial.println();
            workbench::success("Found motors: { ", 0);
            for (const auto& m : *this) {
                Serial.print(m.id); Serial.print(", ");
            }
            CR_PRINT("}\n");
        }

        bool verifyMotor(int8_t motor_id) const {
            for (int8_t i = 0; i < count; i++) {
                if (motorArr[i].id == motor_id) {
                    return true;
                }
            }
            return false;
        }

        Motor& operator[](uint8_t motor_id) {
            for (int8_t i = 0; i < count; i++) {
                if (motorArr[i].id == motor_id) {
                    return motorArr[i];
                }
            }
            CR_PANIC("Motor "); CR_PRINT(motor_id); CR_PRINT(" doesn't exist. Please call motors.findMotors() first.");
            CR_EXIT;
        }

        class iterator {
            Motor* m;
        public:
            explicit iterator(Motor* m) : m(m) {}

            iterator& operator++() {++m; return *this;}
            Motor& operator*() const {return *m;}
            Motor* operator->() const {return m;}
            bool operator==(const iterator& other) const {return m == other.m;}
            bool operator!=(const iterator& other) const {return m != other.m;}
        };

        iterator begin() {return iterator{motorArr};}
        iterator end() {return iterator{motorArr + count};}

    private:
        friend class Goblin;
        MotorList() = default;
    };

}
