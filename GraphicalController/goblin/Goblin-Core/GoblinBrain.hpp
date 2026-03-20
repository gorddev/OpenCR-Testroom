#pragma once
#include <cstdint>

#include "GobUpdate.hpp"
#include "MotorCore.hpp"
#include "errors/gan_err.hpp"
#include "../the-goblin/src/command/command-io.h"
#include "../the-goblin/src/command/packets.h"
#include "serial_port.h"

/* Created by Gordie Novak on 3/15/26.
 * Purpose: 
 * Contains the core components needed to controll the goblin
 */


namespace gobin {

    class GoblinBrain {
    public:
        std::vector<MotorInterface> motors; ///< Motor interface objects interact with.
        GobUpdate updater;                  ///< Determines when we update shit.
        serial_port port;                   ///< The port through which we communicate with the arduino board


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

        void update() {
            // tick the updater.
            updater.tick();

            // fetch information
            fetch();

            // if it's time to update, we update

            send();
        }

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
                case COM_STATUS:
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

        void send() {
            for (int j = 0; j < motors.size(); j++) {

                if (!motors[j].needs_updating) continue;
                motors[j].needs_updating = false;


                if (motors[j].torque != internal_motors[j].torque) { //< torque
                    setTorque(j, motors[j].torque);
                }

                if (motors[j].mode == WHEEL_MODE) {
                    if (motors[j].mode != internal_motors[j].mode) { //< mode
                        setWheelMode(j);
                        setVelocity(j, 0);
                        motors[j].vel = 0;
                    } else if (motors[j].vel != internal_motors[j].vel) { //< velocity
                        setVelocity(j, motors[j].vel);
                        motors[j].vel = internal_motors[j].vel;
                    }
                } else if (motors[j].mode == JOINT_MODE) {
                    if (motors[j].mode != internal_motors[j].mode) { //< mode
                        setJointMode(j);
                        setPosition(j, internal_motors[j].pos);
                        motors[j].pos = internal_motors[j].pos;
                    } else if (motors[j].pos != internal_motors[j].pos) { //< position
                        setPosition(j, motors[j].pos);
                    }
                }
            }
        }

        /* ---------- MotorID ------------ */

        void setMotorID(const uint8_t motor_index, const uint8_t new_motor_index) {
            for (auto& m : internal_motors) {
                if (m.motor_id == new_motor_index) {
                    return;
                }
            }
            port.write(com::bitcast(Command{COM_SET, T_MOTOR_ID, internal_motors[motor_index].motor_id}, new_motor_index));
            internal_motors[motor_index].motor_id = new_motor_index;
            requestMotorScan();

        }

        [[nodiscard]] const MotorCore& getMotor(const u8 motor_index) const {
            return internal_motors[motor_index];
        }

    private:

    /* ---------- Modes ------------ */

        void setWheelMode(const uint8_t motor_index) {
            port.write(com::bitcast(Command{COM_SET, T_MOTOR_WHEEL_MODE, internal_motors[motor_index].motor_id}));
            internal_motors[motor_index].mode = WHEEL_MODE;
        }

        void setJointMode(const uint8_t motor_index) {
            port.write(com::bitcast(Command{COM_SET, T_MOTOR_JOINT_MODE, internal_motors[motor_index].motor_id}));
            internal_motors[motor_index].mode = JOINT_MODE;
        }

    /* ---------- Velocity ------------ */

        void queryVelocity(const uint8_t motor_index) {
            port.write(com::bitcast(Command{COM_GET, T_MOTOR_VELOCITY, internal_motors[motor_index].motor_id}));
        }

        void changeVelocity(const uint8_t motor_index, const int32_t d_velocity) {
            port.write(com::bitcast(Command{COM_CHANGE, T_MOTOR_VELOCITY, internal_motors[motor_index].motor_id}, d_velocity));
            internal_motors[motor_index].vel += d_velocity;
        }

        void setVelocity(const uint8_t motor_index, const int32_t velocity) {
            port.write(com::bitcast(Command{COM_SET, T_MOTOR_VELOCITY, internal_motors[motor_index].motor_id}, velocity));
            internal_motors[motor_index].vel = velocity;
        }

    /* ---------- Position ------------ */

        void queryPosition(const uint8_t motor_index) {
            std::cout << "qp the" << std::endl;
            port.write(com::bitcast(Command{COM_GET, T_MOTOR_VELOCITY, internal_motors[motor_index].motor_id}));
        }

        void changePosition(const uint8_t motor_index, const int32_t d_position) {
            std::cout << "cp the" << std::endl;
            port.write(com::bitcast(Command{COM_CHANGE, T_MOTOR_VELOCITY, internal_motors[motor_index].motor_id}, d_position));
            internal_motors[motor_index].pos += d_position;
        }

        void setPosition(const uint8_t motor_index, const int32_t position) {
            port.write(com::bitcast(Command{COM_SET, T_MOTOR_POSITION, internal_motors[motor_index].motor_id}, position));
            internal_motors[motor_index].pos = position;
        }

    /* ---------- Torque ------------ */
        void setTorque(uint8_t motor_index, bool torque) {
            port.write(com::bitcast(Command{COM_SET, T_MOTOR_TORQUE, internal_motors[motor_index].motor_id}, torque));
            internal_motors[motor_index].torque = torque;
        }

        void requestMotorScan() {
            port.write(com::bitcast(Command{COM_GET, T_MOTOR_LIST}));
        }

        void queryMotors() {
            port.write(com::bitcast(Command{COM_STATUS, T_MOTOR_LIST}));
        }

    /* ********************************************** */
        void unpack_motor_list(const Command& c) {
            bool overwrite = (c.type == COM_INFO);

            const auto dat = reinterpret_cast<const MotorPacket*>(port.data());
            // if we want to overwrite pre-existing motor data.
            if (overwrite || c.num_motors != internal_motors.size()) {
                internal_motors.resize(c.num_motors);
                for (int i = 0; i < c.num_motors; i++) {
                    internal_motors[i].motor_id = dat[i].id;
                    internal_motors[i].model_num = dat[i].model_num;
                    internal_motors[i].motor_id = static_cast<MotorCoreMode>(dat[i].id);
                    internal_motors[i].angle = dat[i].radians;
                    if (dat[i].mode == WHEEL_MODE_P) {
                        internal_motors[i].vel = dat[i].vel;
                        internal_motors[i].mode = WHEEL_MODE;
                    }
                    else if (dat[i].mode == JOINT_MODE_P) {
                        internal_motors[i].pos = dat[i].pos;
                        internal_motors[i].mode = JOINT_MODE;
                    } else {
                        std::cerr << "uninitialized motor" << std::endl;
                    }
                }
                std::ranges::sort(internal_motors, [](const MotorCore& a, const MotorCore& b) {
                    return a.motor_id < b.motor_id;
                });
            } else {
                for (auto & internal_motor : internal_motors) {
                    for (auto& mm: motors) {
                        if (mm.motor_id == internal_motor.motor_id) {
                            mm.motor_id = internal_motor.motor_id;
                            mm.radians = internal_motor.angle;
                            break;
                        }
                    }
                }
            }
            // now we update the motor instance list with the new motors
            motors.clear();
            for (i32 i = 0; i < internal_motors.size(); i++) {
                motors.emplace_back(internal_motors[i], i, overwrite);
            }
        }

    private:
        std::vector<MotorCore> internal_motors;  ///< contains all of our motors.

        explicit GoblinBrain(serial_port port)
            : port(std::move(port)) {}

    };
}
