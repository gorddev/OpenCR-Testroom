#pragma once

#include "bit_array.h"

/* Created by Gordie Novak on 3/16/26.
 * Purpose: 
 * contains all the shared command for serialization purposes*/

namespace goblin {

    // Specifies the beginning of a command
    constexpr char COMMAND_STARTCODE = '\r';
    // Specifies the end of a command
    constexpr char COMMAND_ENDCODE = '\n';

    // Lets us set the target for the command
    enum CommandTarget : unsigned char {
        VELOCITY = 'V',
        POSITION = 'P',
        TORQUE = 'T',
        MODE = 'M'
    };

    // The type of command
    enum CommandType : unsigned char {
        SET = 's',
        CHANGE = 'c',
        GET = 'g',
        INFO = 'i'
    };

    struct Command {
        static void write(void* data, CommandType type, CommandTarget target) {
            unsigned char* bits = reinterpret_cast<unsigned char*>(data);
            bits[0] = COMMAND_STARTCODE;
            bits[1] = type;
            bits[2] = target;
            bits[3] = 0x0;
        }
    };

    template<typename T>
    bit_array<sizeof(Command) + sizeof(T) + 1>
        createCommand(const CommandType type, const CommandTarget target, T data)
    {
        bit_array<sizeof(Command) + sizeof(T) + 1> arr; //< create a bit arr

        Command::write(arr.data(), type, target);       //< write a command

        arr.write(&data, 4);
        arr.write(COMMAND_ENDCODE, arr.size() - 1);

        return arr;
    }

}
