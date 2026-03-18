#pragma once
#include <cstdint>

#include "Motor.hpp"
#include <types/fstring.hpp>

#include "errors/gan_err.hpp"
#include "serial/serial.h"
#include "../../shared/bit-operations/bit_commands.h"
#include "../../shared/bit-operations/bit_buffer_c.hpp"
#include "shared/serial_port.h"

/* Created by Gordie Novak on 3/15/26.
 * Purpose: 
 * Contains the core components needed to controll the goblin
 */


namespace gobin {
    static char port[] = "/dev/tty.debug-console";
    struct GoblinBrain {

        static constexpr uint32_t   buf_size = 300;

        static constexpr uint64_t   baudrate = 1000000;
        static constexpr uint8_t    max_motors = 4;

    private:
        std::vector<Motor> motors;

        serial_port serial;

        bool torque = true;         ///< If torque is enabled or not

    public:
        GoblinBrain() : serial(port, baudrate) {}

        void update() {
            serial.update();

            if (size_t dat = serial.available_data()) {
                auto data = serial.pop_data();
                std::cout << "trying to pop data?";
                mem::bitprint(data.data, data.size);
            }
        }


        void send_serial(const uint8_t* data, size_t size) {
            serial.serial.write(data, size);
        }

        template<typename T>
        void send_command(Command c, T* data) {
            auto bits = gobin::mem::bitcast(c, data);
            serial.serial.write(bits, bits.size());
        }
    };
}
