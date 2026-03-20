#pragma once
#include <iostream>
#include <memory>
#include "../../../the-goblin/src/command/command.h"
#include "errors/gan_log.hpp"
#include "serial/serial.h"
#include <array>


/* Created by Gordie Novak on 3/16/26.
 * Purpose: 
 */

namespace gobin {

    inline std::string port_record;

    class serial_port {
    private:
        std::string path;
        std::unique_ptr<serial::Serial> port;
        std::array<uint8_t, 128> buffer{};

        uint16_t current_command_data_size = false;     //< Whether the serial port knows how large a command will be yet.
        uint8_t has_command_ready = false;              //< Whether the serial port has a command ready for popping.
        uint8_t has_command_header = false;             //< Whether the command header has been found yet. ('~')
        uint8_t bytes_received = 0;


        serial_port(const char port[], uint64_t baudrate)
            : port(std::make_unique<serial::Serial>(port, baudrate, serial::Timeout::simpleTimeout(1000)))
            , path(port){}
    public:

    /* ********************************************************** */
        /** serial_port make function
         * @param port A string giving the absolute path to the port of the serial port you want to access.
         * @param baudrate The baudrate of the device you want to connect to.
         * @return A full serial port if successful, a @code std::nullopt@endcode if not.
         */
        static std::optional<serial_port> make(const char port[], uint64_t baudrate) {

            try {
                serial_port serial(port, baudrate);

                #ifdef __WIN32__
                serial.port->setDTR(true);
                serial.port->setRTS(true);
                #endif

                if (serial.port->isOpen()) {
                    printf("Initialization of port %s @%llu successful.\n", port, baudrate);
                } else {
                    std::cerr << "Failed to open port " << port << std::endl;
                    throw std::runtime_error("Port not opened correctly.");
                }

                std::optional<serial_port> opt;
                opt.emplace(std::move(serial));
                return opt;

            } catch (serial::IOException& e) {
                gan::GAN_WriteLog("serial_port::make()", "Failed to initialize port", port, " with baudrate ", baudrate,
                    ". Serial Library gives error: \n", e.what());
                return std::nullopt;
            } catch (const std::exception& e) {
                gan::GAN_WriteLog("serial_port::make()", "Failed to initialize port", port, " with baudrate ", baudrate,
                    ". Given error: \n", e.what());
                return std::nullopt;
            }
            gan::GAN_WriteLog("serial_port::make()", "__unreachable__");
            return std::nullopt;
        }

    /* ********************************************************** */

        template<uint16_t N>
        void write(const com::Data<N>& data) {
            port->write(data.bits, data.size());
        }

        void write(const uint8_t* data, const uint16_t amount) {
            port->write(data, amount);
        }



    /* ********************************************************** */

        /** Returns the number of bits in the command buffer if a command is ready. 0 otherwise. **/
        [[nodiscard]] uint16_t available() const {
            if (has_command_ready)
                return current_command_data_size;
            return 0;
        }

        /** Gets the current command form the buffer */
        [[nodiscard]] const Command& command() const {
            if (has_command_ready)
                return com::comcast(buffer.data());
            static Command ret{COM_NONE, T_NONE, '\0'};
            return ret;
        }

        [[nodiscard]] const uint8_t* data() const {
            if (has_command_ready)
                return com::comoffset(buffer.data());
            return nullptr;
        }

        /** Returns the raw data in the bitbuffer */
        [[nodiscard]] const uint8_t* bitbuffer() const {
            return buffer.data();
        }

        /** Returns the amount of data in the bitbuffer */
        uint16_t bitwidth() const {
            return current_command_data_size + sizeof(gobin::Command);
        }

        /** Copies all data from the serial buffer into the provided bit buffer.
         * To get the size of the data copied, please call @code available()@endcode*/
        Command pop(uint8_t* buf = nullptr) {
            if (buf != nullptr)
                memcpy(buf,  buffer.data() + sizeof(Command), current_command_data_size);
            current_command_data_size = 0;
            has_command_header = false;
            has_command_ready = false;
            return com::comcast(buffer.data());
        }

    /* ********************************************************** */

        /** Fetches data from the serial buffer. Returns true if a command is ready */
        bool fetch() {
            // if we already have a command ready we won't fetch.
            if (has_command_ready) return true;

            // first we need to get to the command header (the '~' symbol)
            if (!has_command_header) {
                while (port->available() > 0) {
                    uint8_t byte;
                    port->read(&byte, 1);
                    port_record += static_cast<char>(byte);
                    if (byte == '~') { //< this indicates the start of a command.
                        buffer[0] = '~';
                        has_command_header = true;
                        bytes_received = 1;
                        break;
                    } std::cout << "discard: " << byte << std::endl;
                }
            }

            // if we have our command header and currently don't know the size of the command.
            if (has_command_header && !current_command_data_size) {
                size_t cmd_struct_size = sizeof(Command);

                //< if we don't have a full command ready yet, we don't read.
                if (port->available() < (cmd_struct_size - bytes_received))
                    return false;

                // we don't read the first bit because we already have our header '~'
                size_t to_read = cmd_struct_size - bytes_received;
                port->read(buffer.data() + bytes_received, to_read);

                bytes_received += to_read;

                port_record.append(buffer.data() + bytes_received, buffer.data() + bytes_received + to_read);

                // grab the size from our command.

                current_command_data_size = com::comcast(buffer.data()).size - (sizeof(Command) - 1);
            }

            // if we know the current size of our command
            if (current_command_data_size > 0) {
                size_t total_expected = sizeof(Command) + current_command_data_size - 1;

                if (port->available() < (total_expected - bytes_received))
                    return false; //< if we don't have enough bits to fill out command data.

                size_t to_read = total_expected - bytes_received;
                port->read(buffer.data() + bytes_received, to_read);

                port_record.append(buffer.data() + bytes_received, buffer.data() + bytes_received + to_read);

                has_command_ready = true;
                // Reset trackers for the next cycle
                bytes_received = 0;
                return true;
            }

            // if we never do anything, we just return false.
            return false;
        }

        [[nodiscard]] const std::string& get_path() const {
            return path;
        }
    };

    /* ********************************************************** */

}
