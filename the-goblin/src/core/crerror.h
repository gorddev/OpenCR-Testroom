#pragma once

#include <cstdint>

/* Created by Gordie Novak on 3/19/26.
 * Purpose: 
 */



namespace gobin {

    enum CRErrorCode : uint16_t {
        ERROR_UNDEFINED, // [0]
        PRINT_OVERFLOW,
        COMMAND_SIZE_TOO_BIG_FATAL,

        // Logic Errors
        INVALID_COMMAND_TYPE_ECHO_CONSOLE,
        INVALID_COMMAND_TYPE_MOTOR_VELOCITY,
        INVALID_COMMAND_TYPE_MOTOR_POSITION,
        INVALID_COMMAND_TYPE_MOTOR_LIST,
        INVALID_COMMAND_TYPE_CHANGE_ID,
        INVALID_COMMAND_TYPE_SET_JOINT_MODE,
        INVALID_COMMAND_TYPE_SET_WHEEL_MODE,

        UNKNOWN_COMMAND_TYPE,
        INVALID_MOTOR_INDEX_FATAL,

        // Fatal Errors
        POSITION_SET_FATAL,
        POSITION_SET_NOT_JOINT_MODE,

        VELOCITY_SET_FATAL,
        VELOCITY_SET_NOT_WHEEL_MODE,

        JOINT_MODE_SET_FATAL,
        WHEEL_MODE_SET_FATAL,

        SCANNED_FOR_MOTORS_MORE_THAN_ONCE,
    };


    struct Error {
        uint16_t error_id;
        uint8_t motor_id;
    };
}

