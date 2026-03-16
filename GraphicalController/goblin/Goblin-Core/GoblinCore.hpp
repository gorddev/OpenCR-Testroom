#pragma once
#include <cstdint>

#include "Motor.hpp"
#include <types/fstring.hpp>

/* Created by Gordie Novak on 3/15/26.
 * Purpose: 
 * Contains the core components needed to controll the goblin
 */

struct GoblinCore {

    static constexpr uint32_t buf_size = 300;
    static constexpr char port[13] = "/dev/ttyACM0";
    static constexpr uint64_t baudrate = 1000000;
    static constexpr uint8_t num_motors = 4;

    goblin::Motor motors[num_motors];        ///< Velocity the goblin goes for each motor
    gan::fstring<buf_size> serial;   ///< Allows us to send serial data to the pi
    bool torque = true;              ///< If torque is enabled or not

    GoblinCore() { serial.resize<buf_size>(); }

    uint32_t send_serial() {
        size_t s_len = strlen(serial.data());
        //serial.write(port, buf_size);
        serial[0] = '\0';

        return s_len;
    }

};
