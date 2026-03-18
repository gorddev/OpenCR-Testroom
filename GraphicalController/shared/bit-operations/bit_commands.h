#pragma once

#include <type_traits>
#include <__ranges/data.h>

#include "bit_array.h"

/* Created by Gordie Novak on 3/16/26.
 * Purpose: 
 * contains all the shared command for serialization purposes*/


namespace gobin {

    // Specifies the beginning of a command
    constexpr char COMMAND_STARTCODE = '~';
    // Specifies the end of a command
    constexpr char COMMAND_ENDCODE = '\0';

    // The type of command
    enum CommandType : unsigned char {
        SET = 's',
        CHANGE = 'c',
        GET = 'g',
        INFO = 'i'
    };

    // Lets us set the target for the command
    enum CommandTarget : unsigned char {
        VELOCITY = 'V',
        POSITION = 'P',
        TORQUE = 'T',
        MODE = 'M',
        STRING = 'S',
        MOTOR = 'I'
    };

    struct Command {
        uint8_t startCode;
        CommandType type;
        CommandTarget target;
        uint8_t reserved;

        Command() = default;
        Command(CommandType type, CommandTarget target)
            : startCode('~'), type(type), target(target), reserved(':') {
        }

        Command(void* data) {
            const auto bits = static_cast<uint8_t*>(data);
            startCode   = bits[0];
            type        = static_cast<CommandType>(bits[1]);
            target      = static_cast<CommandTarget>(bits[2]);
            reserved    = bits[3];
        }

    };

    namespace mem {
        static void write(void* data, const Command c) {
            unsigned char* bits = reinterpret_cast<unsigned char*>(data);
            bits[0] = COMMAND_STARTCODE;
            bits[1] = c.type;
            bits[2] = c.target;
            bits[3] = 0x0;
        }

        static void write_str(void* data, uint8_t str_len) {
            unsigned char* bits = reinterpret_cast<unsigned char*>(data);
            bits[0] = COMMAND_STARTCODE;
            bits[1] = STRING;
            bits[2] = 0x0;
            bits[3] = 0x0;
        }

        template<typename T>
            requires(std::is_trivially_copyable_v<T>)
        static bit_array<4 + sizeof(T) + 1>
        bitcast(const Command c, T data)
        {
            bit_array<sizeof(Command) + sizeof(T) + 1> arr; //< create a bit arr
            arr.bits[0] = COMMAND_STARTCODE;
            arr.bits[1] = c.type;
            arr.bits[2] = c.target;
            arr.bits[3] = ':';
            arr.write_obj(&data, 4);
            arr.write_byte(COMMAND_ENDCODE, arr.size() - 1);
            return arr;
        }

        template<uint8_t N>
        static bit_array<4 + N + 1>
        bitcast(const char (&str)[N])
        {
            bit_array<4 + N + 1> arr; //< create a bit arr
            arr.bits[0] = COMMAND_STARTCODE;
            arr.bits[1] = STRING;
            arr.bits[2] = N;       //< write a command
            arr.bits[3] = ':';
            arr.write_mem(str, 4, N);
            arr.write_byte(COMMAND_ENDCODE, arr.size() - 1);
            return arr;
        }

        template<size_t N>
        static bit_array<4 + N + 1>
        bitcast(void* data) {
            bit_array<4 + N + 1> arr;
            arr.bits[0] = COMMAND_STARTCODE;
            arr.bits[1] = STRING;
            arr.bits[2] = N;
            arr.bits[3] = ':';
            arr.write_mem(data, 4, N);
            arr.write_byte(COMMAND_ENDCODE, arr.size() - 1);
            return arr;
        }

        template<typename T>
            requires(std::is_aggregate_v<T>)
        void bitread(const void* data, T* obj) {
            std::memcpy(obj, data, sizeof(T));
        }

        inline void bitread(const void* data, void* buffer, size_t num_bits) {
            std::memmove(buffer, data, num_bits);
        }

        inline std::string strread(const void* data) {
            auto str = reinterpret_cast<const char*>(data);
            return std::string(str);
        }

        inline void bitprint(const void* data, size_t num_bits) {
            std::cerr << "==bitprint @" << data << "==\n";
            auto c_data = reinterpret_cast<const char*>(data);
            for (size_t i = 0; i < num_bits; ++i) {
                std::cerr << '[' << i << "] \t| " << c_data[i] +0 << "  \t: " << c_data[i] << "\n";
            }
            if (c_data[num_bits -1 ] == '\0')
                std::cerr << "› \"" << c_data << "\"\n";
            std::cerr << "=============" << std::endl;
        }

    }
;
}
