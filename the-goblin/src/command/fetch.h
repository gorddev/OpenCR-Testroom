#pragma once
#include "../goblin/Goblin.h"
#include "packets.h"

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

            auto cjr = itostr(c.motor_id);
            Vel vel;
            switch (c.type) {
            case COM_GET: // get velocity
                vel = g.motors[c.motor_id].getVelocity();
                CRCommand({COM_INFO, T_MOTOR_VELOCITY, c.motor_id}, &vel, sizeof(Vel));
                break;

            case COM_SET: // set velocity
                vel = *reinterpret_cast<const Vel*>(g.port.data());
                g.motors[c.motor_id].setVelocity(vel);
                break;

            case COM_CHANGE: // change velocity
                vel = *reinterpret_cast<const Vel*>(g.port.data());
                g.motors[c.motor_id].changeVelocity(vel);
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
            const char* str = itostr(c.motor_id);
            switch (c.type) {
            case COM_GET: // get position
                pos = g.motors[c.motor_id].getPosition();
                CRCommand({COM_INFO, T_MOTOR_POSITION,c.motor_id}, &pos, sizeof(Pos));
                break;

            case COM_SET: // set velocity
                pos = *reinterpret_cast<const Pos*>(g.port.data());
                g.motors[c.motor_id].setPosition(pos);

                break;

            case COM_CHANGE: // change velocity
                pos = *reinterpret_cast<const Pos*>(g.port.data());
                g.motors[c.motor_id].changePosition(pos);
                break;

            default: // if command is invalid.
                CRError(INVALID_COMMAND_TYPE_MOTOR_POSITION, c.type);
                break;
            }
        }


        /* ***************** Motor List ************************ */

        static void motor_list(Goblin& g, const Command& c) {
            CRPrint("Retrieving Motor List.");
            if (c.type == COM_GET) {
                MotorListPacket<invar::max_motor_id> motors;

                u8 count = 0;
                for (auto& m: g.motors) {
                    if (count >= g.motors.motor_count)
                        break;
                    motors[count].id = m.id;
                    motors[count].mode = static_cast<MotorModePacket>(m.mode);
                    motors[count].model_num = m.model_num;
                    count++;
                }

                CRCommand({COM_INFO, T_MOTOR_LIST, g.motors.motor_count}, &motors,
                    sizeof(MotorListPacket<>) + sizeof(MotorPacket)*count);
            } else {
                CRError(INVALID_COMMAND_TYPE_MOTOR_LIST, c.type);
            }
        }

        /* ***************** Change ID ************************ */

        static void motor_id(Goblin& g, const Command& c) {
            if (g.port.data() == nullptr)
                return;
            if (c.type == COM_SET) {
                g.motors[c.motor_id].setMotorID(*g.port.data());
            } else CRError(INVALID_COMMAND_TYPE_CHANGE_ID, c.type);
        }

        /* ***************** Joint/Wheel mode ************************ */
        static void joint_mode(Goblin& g, const Command& c) {
            if (c.type == COM_SET) {
                g.motors[c.motor_id].setJointMode(300, 20);
            } else CRError(INVALID_COMMAND_TYPE_SET_JOINT_MODE, c.type);
        }

        static void wheel_mode(Goblin& g, const Command& c) {
            if (c.type == COM_SET) {
                g.motors[c.motor_id].setWheelMode(20);
            } else CRError(INVALID_COMMAND_TYPE_SET_WHEEL_MODE, c.type);
        }
    };
}