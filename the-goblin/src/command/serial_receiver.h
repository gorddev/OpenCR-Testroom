#pragma once

#include "../core/crdef.h"
#include "../core/crstream.h"
#include "command.h"

namespace gobin {

    class serial_receiver {
    public:

    /* ********************************************************** */

        /** Returns the number of bits of a command if a command is ready */
        [[nodiscard]] u16 available() const {
            if (has_command_ready) {
                return current_command_data_size;
            } return 0;
        }

        /** Gets the current command from the buffer **/
        [[nodiscard]] const Command& command() const {
            if (has_command_ready)
                return com::comcast(buffer);
            return Command{COM_NONE};
        }

        /** Gets the data currently stored in the buffer */
        [[nodiscard]] const u8* data() const {
            if (has_command_ready)
                return com::comoffset(buffer);
            return nullptr;
        }

        /** Returns the raw data in the buffer for debugging purposes **/
        [[nodiscard]] const u8* bitbuffer() const {
            return buffer;
        }

        /** Copies all the data from the serial buffer into the provided bit buffer.
         * To get the size of the data that will be copies, please call @code available@endcode */
        Command pop(u8* buf = nullptr) {
            if (buf != nullptr)
                memcpy(buf,  buffer + sizeof(Command), current_command_data_size);
            current_command_data_size = 0;
            has_command_header = false;
            has_command_ready = false;
            bytes_received = 0;
            return com::comcast(buffer);
        }

    /* ********************************************************** */

        /** Fetches data from the serial buffer. Returns true if a command is ready. **/
        b8 fetch() {
            // if we already have a command ready we won't fetch.
            if (has_command_ready) return true;

            // first we need to get to the command header (the '~' symbol)
            if (!has_command_header) {
                while (Serial.available() > 0) {
                    uint8_t byte;
                    byte = Serial.read();
                    if (byte == '~') { //< this indicates the start of a command.
                        buffer[0] = '~';
                        has_command_header = true;
                        bytes_received = 1;
                        break;
                    } CRPrint("Discarding");
                }
            }

            // if we have our command header and currently don't know the size of the command.
            if (has_command_header && !current_command_data_size) {
                size_t cmd_struct_size = sizeof(Command);

                //< if we don't have a full command ready yet, we don't read.
                if (Serial.available() < (cmd_struct_size - bytes_received))
                    return false;

                // we don't read the first bit because we already have our header '~'
                size_t to_read = cmd_struct_size - bytes_received;
                Serial.readBytes(buffer + bytes_received, to_read);

                bytes_received += to_read;

                // grab the size from our command.

                current_command_data_size = com::comcast(buffer).size - (sizeof(Command) - 1);
            }

            // if we know the current size of our command
            if (current_command_data_size > 0) {
                size_t total_expected = sizeof(Command) + current_command_data_size - 1;

                if (Serial.available() < (total_expected - bytes_received))
                    return false; //< if we don't have enough bits to fill out command data.

                size_t to_read = total_expected - bytes_received;
                Serial.readBytes(buffer + bytes_received, to_read);

                has_command_ready = true;
                // Reset trackers for the next cycle
                bytes_received = 0;
                return true;
            }

            // if we never do anything, we just return false.
            return false;
        }

    /* ********************************************************** */

        static constexpr u16 buf_size = 64;
    private:
        u16 current_command_data_size = 0;
        u16 bytes_received = 0;
        b8 has_command_ready = false;
        b8 has_command_header = false;

        u8 buffer[buf_size]{};
    };

}
