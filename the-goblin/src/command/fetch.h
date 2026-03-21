#pragma once
#include "../goblin/Goblin.h"
#include "packets.h"
#include "../core/crconnect.h"

void loop();

namespace gobin {
    class fetch {

        friend void ::loop();


        /* ***************** Console Printouts ************************ */

        static void echo_console(const Goblin& g, const Command& c) {
            if (c.type == COM_ECHO || c.type == COM_STRING || c.type == COM_INFO) {
                CRPrint(g.port.data(), g.port.available());
            } else CRError(INVALID_COMMAND_TYPE_ECHO_CONSOLE, c.type);
        }

        /* ***************** Motor Velocity ************************ */

        static void motor_velocity(Goblin& g, const Command& c) {
            if (!g.port.data())
                return;

            Vel vel;
            const Motor* mot = g.motors.at(c.motor_id);
            if (!mot) {
                CRError(FETCH_INVALID_MOTOR_ID, c.motor_id);
                return;
            }

            switch (c.type) {
            case COM_GET: // get velocity
                vel = mot->getVelocity();
                CRCommand({COM_INFO, T_MOTOR_VELOCITY, c.motor_id}, &vel, sizeof(Vel));
                break;
            case COM_SET: // set velocity
                vel = *reinterpret_cast<const Vel*>(g.port.data());
                mot->setVelocity(vel);
                break;
            case COM_CHANGE: // change velocity
                vel = *reinterpret_cast<const Vel*>(g.port.data());
                mot->changeVelocity(vel);
                break;
            default: // if command is invalid.
                CRError(INVALID_COMMAND_TYPE_MOTOR_VELOCITY, c.type);
                break;
            }
        }

        /* ***************** Motor Position ************************ */

        static void motor_position(Goblin& g, const Command& c) {
            if (!g.port.data())
                return;

            Pos pos;
            const Motor* mot = g.motors.at(c.motor_id);
            if (!mot) {
                CRError(FETCH_INVALID_MOTOR_ID, c.motor_id);
                return;
            }

            switch (c.type) {
            case COM_GET: // get position
                pos = mot->getPosition();
                CRCommand({COM_INFO, T_MOTOR_POSITION,c.motor_id}, &pos, sizeof(Pos));
                break;
            case COM_SET: // set position
                pos = *reinterpret_cast<const Pos*>(g.port.data());
                mot->setPosition(pos);
                break;
            case COM_CHANGE: // change position
                pos = *reinterpret_cast<const Pos*>(g.port.data());
                mot->changePosition(pos);
                break;
            default: // if command is invalid.
                CRError(INVALID_COMMAND_TYPE_MOTOR_POSITION, c.type);
                break;
            }
        }


        /* ***************** Motor List ************************ */

        static void motor_list(Goblin& g, const Command& c) {
            if (c.type == COM_GET) {
                g.motors.findMotors();
            } else if (c.type == COM_STATUS) {
                g.motors.sendSerializedMotors(false);
            } else {
                CRError(INVALID_COMMAND_TYPE_MOTOR_LIST, c.type);
            }
        }

        /* ***************** Change ID ************************ */

        static void motor_id(Goblin& g, const Command& c) {
            if (g.port.data() == nullptr)
                return;
            if (c.type == COM_SET) {
                g.motors.changeMotorID(c.motor_id, *g.port.data());
            } else CRError(INVALID_COMMAND_TYPE_CHANGE_ID, c.type);
        }

        /* ***************** Joint/Wheel mode ************************ */
        static void joint_mode(Goblin& g, const Command& c) {
            if (c.type == COM_SET) {
                if (Motor* mot = g.motors.at(c.motor_id)) {
                    mot->setJointMode(invar::vel_rate_joint, invar::acc_rate);
                } else CRError(FETCH_INVALID_MOTOR_ID, c.motor_id);
            } else CRError(INVALID_COMMAND_TYPE_SET_JOINT_MODE, c.type);
        }

        static void wheel_mode(Goblin& g, const Command& c) {
            if (c.type == COM_SET) {
                if (Motor* mot = g.motors.at(c.motor_id)) {
                    mot->setWheelMode(invar::acc_rate);
                } else CRError(FETCH_INVALID_MOTOR_ID, c.motor_id);
            } else CRError(INVALID_COMMAND_TYPE_SET_WHEEL_MODE, c.type);
        }


        /* ****************** Torque **************************** */
        static void torque(Goblin& g, const Command& c) {
            if (c.type == COM_SET) {
                if (Motor* mot = g.motors.at(c.motor_id)) {
                    mot->setTorque(*g.port.data());
                } else CRError(FETCH_INVALID_MOTOR_ID, c.motor_id);
            } else CRError(INVALID_COMMAND_TYPE_SET_TORQUE, c.type);
        }

        /* ****************** Reset **************************** */
        static void reset(Goblin& g) {
            for (auto& m: g.motors) {
                m.disableTorque();
            } serial::arduino_hard_reset();
        }
    };
}