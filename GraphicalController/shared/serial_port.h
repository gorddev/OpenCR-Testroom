#pragma once
#include <iostream>
#include <memory>
#include "bit-operations/bit_buffer_c.hpp"
#include "bit-operations/bit_commands.h"
#include "errors/gan_err.hpp"
#include "serial/serial.h"

/* Created by Gordie Novak on 3/16/26.
 * Purpose: 
 */

namespace gobin {

    struct serial_port {

        serial::Serial serial;
        uint8_t serial_overflow[50];
        uint8_t serial_overflow_size = 0;
        bit_buffer_c<30, 50> command_queue;


        serial_port(const char port[], uint64_t baudrate)
            : serial(port, baudrate, serial::Timeout::simpleTimeout(1000)), serial_overflow{}
        {
            auto arr = mem::bitcast("Connection");
            mem::bitprint(arr.bits, arr.size());
            std::cout << std::flush;

            if (serial.isOpen()) {
                std::cout << "Serial to " << port << " w/baud " << baudrate << " opened successfully" << std::endl;
            } else {
                gan::err::panic("GoblinBrain()::GoblinBrain()", "Serial to ", port, "failed to open.");
            }
            serial.write(arr.data(), arr.size());

            serial_overflow[0] = '\0';
        }

        void update() {
            if (const size_t bit_num = serial.available()) {
                const size_t arr_size = bit_num + serial_overflow_size;
                uint8_t read_bits[arr_size];
                size_t cur_size = 0;
                // if we have remaining bits
                if (serial_overflow[0] != '\0') {
                    cur_size = serial_overflow_size;
                    memcpy(read_bits, serial_overflow, cur_size);
                    serial_overflow_size = 0;
                    serial_overflow[0] = '\0';
                }

                serial.read(read_bits + cur_size, bit_num);

                size_t last_size = 0, i;
                std::cerr << "reading: ";
                for (i = cur_size; i < arr_size; i++) {
                    std::cerr << " " << read_bits[i];
                    if (i == '~') {
                        command_queue.push(read_bits + last_size, i - last_size);
                        last_size = i;
                    }
                }
                std::cerr << std::endl;

                if (read_bits[i] != '\0') {
                    memcpy(serial_overflow, read_bits + i, i - last_size);
                    serial_overflow_size = i - last_size;
                }
            }
        }

        size_t available_data() const {
            return command_queue.size();
        }

        dataPair pop_data() {
            return command_queue.pop();
        }
    };

}
