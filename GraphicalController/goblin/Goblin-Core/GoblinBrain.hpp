#pragma once
#include <cstdint>

#include "Motor.hpp"
#include "errors/gan_err.hpp"
#include "../the-goblin/src/command/command-io.h"
#include "../the-goblin/src/command/packets.h"
#include "shared/serial_port.h"

/* Created by Gordie Novak on 3/15/26.
 * Purpose: 
 * Contains the core components needed to controll the goblin
 */


namespace gobin {

    class GoblinBrain {
    public:
        std::vector<Motor> motors;  ///< contains all of our motors.
        serial_port port;         ///< The port through which we communicate with the arduino board

        explicit GoblinBrain(serial_port port)
            : port(std::move(port)) {}

    public:
        /** @param port The file with which you would like to connect to.
         * @param baudrate The baudrate of the connection. Default for dynamixel is 1000000
         * @return An optional containing a GoblinBrain. */
        static std::optional<GoblinBrain> make(const char port[], uint64_t baudrate = 1000000) {
            auto opt = serial_port::make(port, baudrate);
            if (!opt) {
                gan::GAN_AppendLog("GoblinBrain::make()", "Failed to create serialization port ", port);
                return std::nullopt;
            } return GoblinBrain(std::move(opt.value()));
        }

    /* ********************************************** */

        void fetch() {
            while (port.fetch()) {
                if (!port.available())
                    break;



                switch (Command c = port.command(); c.type) {
                /* --------------- */
                case COM_ERROR:
                    com::printerr(c, com::datacast<Error>(port.data()));
                    break;
                /* --------------- */
                case COM_STRING:
                    std::cout << "|=CR=|::[" << port.data() << "]" << std::endl;
                    break;
                /* --------------- */
                case COM_INFO:
                    if (c.target == T_MOTOR_LIST) {
                        unpack_motor_list(c);
                        break;
                    }
                /* --------------- */
                default:
                    break;
                }

                port.pop();
            }

        }

    /* ---------- Modes ------------ */

        void setWheelMode(const uint8_t motor_index) {
            port.write(com::bitcast(Command{COM_SET, T_MOTOR_WHEEL_MODE, motors[motor_index].id}));
        }

        void setJointMode(const uint8_t motor_index) {
            port.write(com::bitcast(Command{COM_SET, T_MOTOR_JOINT_MODE, motors[motor_index].id}));
        }

    /* ---------- Velocity ------------ */

        void queryVelocity(const uint8_t motor_index) {
            port.write(com::bitcast(Command{COM_GET, T_MOTOR_VELOCITY, motors[motor_index].id}));
        }

        void changeVelocity(const uint8_t motor_index, const int32_t d_velocity) {
            port.write(com::bitcast(Command{COM_CHANGE, T_MOTOR_VELOCITY, motors[motor_index].id}, d_velocity));
            motors[motor_index].vel += d_velocity;
        }

        void setVelocity(const uint8_t motor_index, const int32_t velocity) {
            port.write(com::bitcast(Command{COM_SET, T_MOTOR_VELOCITY, motors[motor_index].id}, velocity));
            motors[motor_index].vel = velocity;
        }

    /* ---------- Position ------------ */

        void queryPosition(const uint8_t motor_index) {
            port.write(com::bitcast(Command{COM_GET, T_MOTOR_VELOCITY, motors[motor_index].id}));
        }

        void changePosition(const uint8_t motor_index, const int32_t d_position) {
            port.write(com::bitcast(Command{COM_CHANGE, T_MOTOR_VELOCITY, motors[motor_index].id}, d_position));
            motors[motor_index].pos += d_position;
        }

        void setPosition(const uint8_t motor_index, const int32_t position) {
            port.write(com::bitcast(Command{COM_SET, T_MOTOR_POSITION, motors[motor_index].id}, position));
            motors[motor_index].pos = position;
        }

    /* ---------- MotorID ------------ */

        void changeMotorID(uint8_t motor_index, uint8_t new_motor_index) {
            port.write(com::bitcast(Command{COM_SET, T_MOTOR_ID, motors[motor_index].id}, new_motor_index));
        }

        [[nodiscard]] const std::vector<Motor>& getMotors() const {
            return motors;
        }

        void queryMotorList() {
            port.write(com::bitcast(Command{COM_GET, T_MOTOR_LIST}));
        }

    /* ---------- G

    /* ********************************************** */
    private:
        void unpack_motor_list(const Command& c) {
            auto dat = reinterpret_cast<const MotorPacket*>(port.data());
            motors.resize(c.num_motors);
            com::comprint(port.bitbuffer());
            for (int i = 0; i < c.num_motors; i++) {
                motors[i].id = dat[i].id;
                motors[i].model_num = dat[i].model_num;
                motors[i].mode = dat[i].mode;
            }
            std::ranges::sort(motors, [](const Motor& a, const Motor& b) {
                return a.id < b.id;
            });
        }


    public:
        [[nodiscard]] const serial_port& get_port() const {
            return port;
        }
    };
}
