#include "GoblinBrain.hpp"

#include "Verify_Goblin.hpp"

using namespace gobin;

GoblinBrain::GoblinBrain(const char port[], uint64_t baudrate) {
    if (port != nullptr) {
        this->port.open(port, baudrate, this->brain_log);
    }
}

/* ********************************************** */

void GoblinBrain::update() {
    updater.tick();
    fetch();
    send();
}

void GoblinBrain::fetch() {
    if (!port.is_open())
        return;

    try {
        while (port.fetch()) {
            if (!port.available())
                break;

            switch (Command c = port.command(); c.type) {
                /* --------------- */
            case COM_ERROR:
                open_cr_log.record(".CR › Error!\n=| ",
                    com::err_str(com::datacast<Error>(port.data())),
                    "\n=| Spec: {", port.command().reserved, "}");
                break;
                /* --------------- */
            case COM_STRING:
                open_cr_log.record(".CR › \"", reinterpret_cast<const char*>(port.data()), "\"");
                break;
                /* --------------- */
            case COM_STATUS:
            case COM_INFO:
                if (c.target == T_MOTOR_LIST) {
                    unpack_motor_list(c);
                    break;
                } if (c.target == T_RESET) {
                    port.close();
                    open_cr_log.clear();
                    brain_log.record("Reset OpenCR board.");
                }
                /* --------------- */
            default:
                break;
            }

            port.pop();
        }
    } catch (serial::IOException& e) {
        brain_log.record("GoblinBrain fetch failed with error: ", e.what(), "\nDisconnecting now");
        port.close();
        open_cr_log.clear();
    }
}

void GoblinBrain::send() {
    if (!port.is_open())
        return;

    try {
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
    } catch (std::exception& e) {
        brain_log.record("GoblinBrain::send() failed with error: ", e.what(), ". Closing port connection.");
        port.close();
        open_cr_log.clear();
    }
}

/* ---------- MotorID ------------ */

void GoblinBrain::setMotorID(const uint8_t motor_index, const uint8_t new_motor_index) {
    for (auto& m : internal_motors) {
        if (m.motor_id == new_motor_index) {
            return;
        }
    }
    port.write(com::bitcast(Command{COM_SET, T_MOTOR_ID, internal_motors[motor_index].motor_id}, new_motor_index));
    internal_motors[motor_index].motor_id = new_motor_index;
    requestMotorScan();

}

void GoblinBrain::requestMotorScan() {
    port.write(com::bitcast(Command{COM_GET, T_MOTOR_LIST}));
}

void GoblinBrain::queryMotors() {
    port.write(com::bitcast(Command{COM_STATUS, T_MOTOR_LIST}));
}

bool GoblinBrain::openPort(const char port[], uint64_t baudrate) {
    if (this->port.open(port, baudrate, brain_log)) {
        data::writeConfigData(port, baudrate, brain_log);
        return true;
    } return false;
}

void GoblinBrain::closePort() {
    port.close(&brain_log);
    open_cr_log.clear();
}

void GoblinBrain::resetArduino() {
    port.write(com::bitcast(Command{COM_SET, T_RESET}));
}

bool GoblinBrain::is_connected() {
    return port.is_open();
}

const std::string& GoblinBrain::getPortPath() {
    return port.get_path();
}

[[nodiscard]] const MotorCore& GoblinBrain::getMotorCore(const u8 motor_index) const {
    return internal_motors[motor_index];
}

void GoblinBrain::flagAllMotorsForUpdate() {
    for (auto& m : motors) {
        m.flagForUpdate();
    }
}


/* ---------- Modes ------------ */

void GoblinBrain::setWheelMode(const uint8_t motor_index) {
    port.write(com::bitcast(Command{COM_SET, T_MOTOR_WHEEL_MODE, internal_motors[motor_index].motor_id}));
    internal_motors[motor_index].mode = WHEEL_MODE;
}

void GoblinBrain::setJointMode(const uint8_t motor_index) {
    port.write(com::bitcast(Command{COM_SET, T_MOTOR_JOINT_MODE, internal_motors[motor_index].motor_id}));
    internal_motors[motor_index].mode = JOINT_MODE;
}

/* ---------- Velocity ------------ */

void GoblinBrain::queryVelocity(const uint8_t motor_index) {
    port.write(com::bitcast(Command{COM_GET, T_MOTOR_VELOCITY, internal_motors[motor_index].motor_id}));
}

void GoblinBrain::changeVelocity(const uint8_t motor_index, const int32_t d_velocity) {
    port.write(com::bitcast(Command{COM_CHANGE, T_MOTOR_VELOCITY, internal_motors[motor_index].motor_id}, d_velocity));
    internal_motors[motor_index].vel += d_velocity;
}

void GoblinBrain::setVelocity(const uint8_t motor_index, const int32_t velocity) {
    port.write(com::bitcast(Command{COM_SET, T_MOTOR_VELOCITY, internal_motors[motor_index].motor_id}, velocity));
    internal_motors[motor_index].vel = velocity;
}

/* ---------- Position ------------ */

void GoblinBrain::queryPosition(const uint8_t motor_index) {
    port.write(com::bitcast(Command{COM_GET, T_MOTOR_VELOCITY, internal_motors[motor_index].motor_id}));
}

void GoblinBrain::changePosition(const uint8_t motor_index, const int32_t d_position) {
    port.write(com::bitcast(Command{COM_CHANGE, T_MOTOR_VELOCITY, internal_motors[motor_index].motor_id}, d_position));
    internal_motors[motor_index].pos += d_position;
}

void GoblinBrain::setPosition(const uint8_t motor_index, const int32_t position) {
    port.write(com::bitcast(Command{COM_SET, T_MOTOR_POSITION, internal_motors[motor_index].motor_id}, position));
    internal_motors[motor_index].pos = position;
}

/* ---------- Torque ------------ */
void GoblinBrain::setTorque(uint8_t motor_index, bool torque) {
    port.write(com::bitcast(Command{COM_SET, T_MOTOR_TORQUE, internal_motors[motor_index].motor_id}, torque));
    internal_motors[motor_index].torque = torque;
}

/* ********************************************** */
void GoblinBrain::unpack_motor_list(const Command& c) {
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
                brain_log.record("WARN: Motor with unknown mode added: [", dat[i].id, "]");
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