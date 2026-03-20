#pragma once
#include "Workbench.h"

namespace gobin {

    class Goblin;

    enum MotorMode : u8 {
        NO_MOTOR_MODE = 0,
        WHEEL_MODE = 1,
        JOINT_MODE = 2
    };

    struct Motor {
        u16 model_num = 0;
        u8 id = -1;
        MotorMode mode = NO_MOTOR_MODE;

        void setVelocity(i32 velocity) const {
            if (mode != WHEEL_MODE) {
                CRPrint("Warning: Cannot set velocity as motor is not in wheel mode.\n");
                return;
            }
            if (velocity > invar::vel_max)
                velocity = invar::vel_max;
            else if (velocity < -invar::vel_max)
                velocity = -invar::vel_max;

            if (!workbench::wb.goalVelocity(id, velocity)) {
                CRPrint("Failed to set velocity mode for motor.\n");
                CRExit();
            }
        }

        void changeVelocity(i32 d_velocity) const {
            const i32 vel = getVelocity();
            setVelocity(vel + d_velocity);
        }

        void changePosition(i32 position) const {
            const i32 pos = getPosition();
            setPosition(pos + position);
        }

        void setPosition(i32 position) const {
            if (mode == JOINT_MODE) {
                if (!workbench::wb.goalPosition(id, position)) {
                    CRError(POSITION_SET_FATAL, id);
                    CRExit();
                }
            } else {
                CRError(POSITION_SET_NOT_JOINT_MODE, id);
            }
        }

        i32 getVelocity() const {
            i32 vel;
            workbench::wb.getPresentVelocityData(id, &vel);
            return vel;
        }

        i32 getPosition() const {
            i32 pos;
            workbench::wb.getPresentPositionData(id, &pos);
            return pos;
        }

        float getRadians() const {
            float radians;
            workbench::wb.getRadian(id, &radians);
            return radians;
        }

        void setJointMode(const i32 vel, const i32 acc) {
            if (!workbench::wb.jointMode(id, vel, acc)) {
                CRError(JOINT_MODE_SET_FATAL, id);
                CRExit();
            }
            mode = JOINT_MODE;
        }

        void setWheelMode(u32 vel) {
            if (!workbench::wb.wheelMode(id, vel)) {
                CRError(WHEEL_MODE_SET_FATAL, id);
                CRExit();
            }
            mode = WHEEL_MODE;
        }

        void setMotorID(const u8 motor_id) const {
            workbench::wb.changeID(id, motor_id);
        }

        void enableTorque() const {
            workbench::wb.torqueOn(id);
        }

        void disableTorque() const {
            workbench::wb.torqueOff(id);
        }

        Motor() = default;

    };

    struct MotorList {
    private:
        Motor motorArr[invar::max_motor_id];
    public:
        u8 motor_count = 0;

        void addMotor(i8 motor_id, u16 model_num) {
            if (motor_count < invar::max_motor_id) {
                motorArr[motor_count].id = motor_id;
                motorArr[motor_count].model_num = model_num;
                motor_count++;
            }
        }

        b8 findMotors() {
            if (motor_count != 0) {
                CRError(SCANNED_FOR_MOTORS_MORE_THAN_ONCE, motor_count);
                return false;
            }

            u16 model_num;

            CRPrint("Checking for motor ids.");
            for (i8 id = 0; id < invar::max_motor_id; id++) {
                if (workbench::wb.ping(id, &model_num, &workbench::cr_log)) {
                    // if we find the motor, add it to our list of found motors.
                    addMotor(id, model_num);
                }
            }

            CRPrint("Found motors.");
            CRCommand({COM_INFO, T_MOTOR_LIST, motor_count}, motorArr, sizeof(Motor) * motor_count);

            return true;
        }

        b8 verifyMotor(i8 motor_id) const {
            for (i8 i = 0; i < motor_count; i++) {
                if (motorArr[i].id == motor_id) {
                    return true;
                }
            }
            return false;
        }

        Motor& operator[](u8 motor_id) {
            for (i8 i = 0; i < motor_count; i++) {
                if (motorArr[i].id == motor_id) {
                    return motorArr[i];
                }
            }
            CRError(INVALID_MOTOR_INDEX_FATAL, motor_id);
            CRPanic("Motor doesn't exist. Please call motors.findMotors() first");
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

    private:
        friend class Goblin;
        MotorList() = default;
    };

}
