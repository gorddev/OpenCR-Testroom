#pragma once

#include "command.h"
#include <algorithm>
#include <iostream>

#include "packets.h"
#include "../core/crerror.h"

namespace gobin::com {


/* ********************************************* */
    /// Converts a com::ComType into a string.
    inline const char* type_str(ComType c) {
        switch (c) {
        case COM_GET:
            return "GET";
        case COM_SET:
            return "SET";
        case COM_CHANGE:
            return "CHANGE";
        case COM_STRING:
            return "STRING";
        case COM_INFO:
            return "INFO";
        case COM_ERROR:
            return "ERROR";
        case COM_ECHO:
            return "ECHO";
        default:
            return "__undefined__";
        }
    }

    /// Converts a com::ComTarget into a string.
    inline const char* target_str(ComTarget t) {
        switch (t) {
        case T_PRINT_CONSOLE:
            return "PRINT_CONSOLE";
        case T_MOTOR_VELOCITY:
            return "MOTOR_VELOCITY";
        case T_MOTOR_POSITION:
            return "MOTOR_POSITION";
        case T_STATUS:
            return "STATUS";
        case T_MOTOR_LIST:
            return "MOTOR_LIST";
        case T_MOTOR_JOINT_MODE:
            return "MOTOR_JOIN_MODE";
        case T_MOTOR_WHEEL_MODE:
            return "MOTOR_WHEEL_MODE";
        case T_MOTOR_ID:
            return "MOTOR_ID";
        case T_NONE:
            return "NONE";
        }
        return "__undefined__";
    }


/* ********************************************* */

    /// Displays chars, including escape characters.
    inline void display_esc_code(const char &c) {
        switch (c) {
        case '\n':
            std::cout << "\\n";
            break;
        case '\r':
            std::cout << "\\r";
            break;
        case '\0':
            std::cout << "\\0";
            break;
        case '\t':
            std::cout << "\\t";
            break;
        case '\f':
            std::cout << "\\f";
            break;
        case '\a':
            std::cout << "\\a";
            break;
        case '\v':
            std::cout << "\\v";
            break;
        case '\b':
            std::cout << "\\b";
            break;
        default:
            std::cout << c;
            break;
        }
    }

/* ********************************************* */

    /// Prits all the raw bits of a byte buffer.
    inline void bitprint(const void *bytes, uint16_t size) {
        auto* raw_data = reinterpret_cast<const uint8_t *>(bytes);
        std::cout << "========================\n==bitread @"
                  << reinterpret_cast<const void *>(raw_data) << "==\n"
                  << " index\t| deci\t| char\n"
                  << "————————————————————————\n";
        constexpr uint16_t col = 10;
        constexpr uint16_t row = 5;
        constexpr uint16_t max = col * row;
        uint16_t run_size = size;
        for (uint16_t r = 0; r * col < size; r += row) {
            for (uint16_t i = 0; i < col; i++) {
                for (uint16_t k = 0; i + col * (k) < std::min(run_size, max); k++) {
                    std::cout << "[" << i + (r + k) * col
                              << "]\t: " << raw_data[i + (r + k) * col] + 0 << "  \t: \'";
                    display_esc_code(raw_data[i + (r + k) * col]);
                    std::cout << "\'\t|  ";
                }
                std::cout << '\n';
            }
            std::cout << "————————————————————————\n";
            run_size -= max;
        }
        if (raw_data[size - 1] == '\0') {
            std::cout << "> ";
            for (int i = 0; i < size; i++) {
                display_esc_code(raw_data[i]);
            }
            std::cout << std::endl;
        }

        std::cout << "==============" << std::endl;
    }

    /// Alternate function call to bitprint that takes in a data obj.
    template<uint16_t N>
    void bitprint(const com::Data<N>& data) {
        bitprint(data.bits, N);
    }


/* ********************************************* */

    /// Prints a command along with all of its raw data.
    inline void comprint(const Command& c, const uint8_t* raw_data) {
        std::cout << "==Command @"<< reinterpret_cast<const void*>(raw_data) << "==\n"
                     "Type\t[" << c.type+0 << "]   \t> " << com::type_str(c.type)
                  << "\nTarget\t[" << c.target+0 << "]\t> " << target_str(c.target)
                  << "\nSpecifier   \t> " << c.reserved+0
                  << "\nSize        \t> " << c.size << "\n"
                     "========================\n"
                  << std::hex;

        if (static_cast<uint8_t>(c.type) != static_cast<uint8_t>(COM_STRING)) {
            for (int i = 0; i < c.size - sizeof(Command); i++) {
                std::cout << raw_data[i]+0 << " ";
                if ((i+1)%10 == 0 && i+1 != c.size) std::cout << "\n";
            }
        } else {
            std::cout << raw_data;
        }
        std::cout << "\n========================\n" << std::dec;
    }

    /// Alternative function call for print.
    inline void comprint(const uint8_t* data) {
        comprint(comcast(data), data + sizeof(Command));
    }




/* ********************************************* */

    inline const char* err_str(Error e) {
        switch (e.error_id) {
        case ERROR_UNDEFINED:
            return "ERROR_UNDEFINED";
        case PRINT_OVERFLOW:
            return "PRINT_OVERFLOW";
        case COMMAND_SIZE_TOO_BIG_FATAL:
            return "COMMAND_SIZE_TOO_BIG_FATAL";
        case INVALID_COMMAND_TYPE_CHANGE_ID:
            return "INVALID_COMMAND_TYPE_CHANGE_ID";
        case INVALID_COMMAND_TYPE_ECHO_CONSOLE:
            return "INVALID_COMMAND_TYPE_ECHO_CONSOLE";
        case INVALID_COMMAND_TYPE_MOTOR_LIST:
            return "INVALID_COMMAND_TYPE_MOTOR_LIST";
        case INVALID_COMMAND_TYPE_MOTOR_POSITION:
            return "INVALID_COMMAND_TYPE_MOTOR_POSITION";
        case INVALID_COMMAND_TYPE_MOTOR_VELOCITY:
            return "INVALID_COMMAND_TYPE_MOTOR_VELOCITY";
        case INVALID_COMMAND_TYPE_SET_JOINT_MODE:
            return "INVALID_COMMAND_TYPE_SET_JOINT_MODE";
        case INVALID_COMMAND_TYPE_SET_WHEEL_MODE:
            return "INVALID_COMMAND_TYPE_SET_WHEEL_MODE";
        case UNKNOWN_COMMAND_TYPE:
            return "UNKNOWN_COMMAND_TYPE";
        case INVALID_MOTOR_INDEX_FATAL:
            return "INVALID_MOTOR_INDEX_FATAL";
        case POSITION_SET_FATAL:
            return "POSITION_SET_FATAL";
        case POSITION_SET_NOT_JOINT_MODE:
            return "POSITION_SET_NOT_JOINT_MODE";
        case VELOCITY_SET_FATAL:
            return "VELOCITY_SET_FATAL";
        case VELOCITY_SET_NOT_WHEEL_MODE:
            return "VELOCITY_SET_NOT_WHEEL_MODE";
        case JOINT_MODE_SET_FATAL:
            return "JOINT_MODE_SET_FATAL";
        case WHEEL_MODE_SET_FATAL:
            return "WHEEL_MODE_SET_FATAL";
        case SCANNED_FOR_MOTORS_MORE_THAN_ONCE:
            return "SCANNED_FOR_MOTORS_MORE_THAN_ONCE";
        default:
            return "__undefined__";
        }
    }

    inline void printerr(const Command& c, const Error& e) {
        std::cout << "== Command Err => \'" << err_str(e) << "\' spec: [" << e.motor_id+0 << "]" << std::endl;
    }

} // namespace gobin::com