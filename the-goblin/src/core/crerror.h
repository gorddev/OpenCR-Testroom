#pragma once

#include <cstdint>

/* Created by Gordie Novak on 3/19/26.
 * Purpose: 
 */



namespace gobin {

    enum CRErrorCode : uint16_t {
        // Memory corruption errors
        ERROR_UNDEFINED, // [0]
        PRINT_OVERFLOW,
        COMMAND_SIZE_TOO_BIG_FAIL,

        // Initialization errors
        DXL_WORKBENCH_ALREADY_INITIALIZED,

        // Logic Errors
        INVALID_COMMAND_TYPE_ECHO_CONSOLE,
        INVALID_COMMAND_TYPE_MOTOR_VELOCITY,
        INVALID_COMMAND_TYPE_MOTOR_POSITION,
        INVALID_COMMAND_TYPE_MOTOR_LIST,
        INVALID_COMMAND_TYPE_CHANGE_ID,
        INVALID_COMMAND_TYPE_SET_JOINT_MODE,
        INVALID_COMMAND_TYPE_SET_WHEEL_MODE,
        INVALID_COMMAND_TYPE_SET_TORQUE,

        UNKNOWN_COMMAND_TYPE,

        /*- Motor Errors -*/
        // position
        POSITION_SET_FAIL,
        POSITION_GET_FAIL,
        POSITION_SET_ABOVE_MAX,
        POSITION_SET_BELOW_MIN,
        POSITION_SET_NOT_JOINT_MODE,
        // velocity
        VELOCITY_SET_FAIL,
        VELOCITY_GET_FAIL,
        VELOCITY_SET_ABOVE_MAX,
        VELOCITY_SET_BELOW_MIN,
        VELOCITY_SET_NOT_WHEEL_MODE,
        // radians
        RADIANS_GET_FAIL,
        // modes
        JOINT_MODE_SET_FAIL,
        WHEEL_MODE_SET_FAIL,
        // misc
        MOTOR_ID_SET_FAIL,
        TORQUE_ENABLE_FAIL,
        TORQUE_DISABLE_FAIL,

        /*- MotorList -*/
        SCANNED_FOR_MOTORS_MORE_THAN_ONCE,
        NO_MOTORS_FOUND_IN_SCAN,
        MOTOR_DOES_NOT_EXIST,
        INVALID_NEW_MOTOR_ID_ABOVE_LIMIT,
        INVALID_NEW_MOTOR_ID_MOTOR_DOES_NOT_EXIST,
        INVALID_NEW_MOTOR_ID_MOTOR_ALREADY_HAS_ID,

        /*- Deserialization -*/
        FETCH_INVALID_MOTOR_ID,

    };


    struct Error {
        uint16_t error_id;
        uint8_t motor_id;
    };
}

