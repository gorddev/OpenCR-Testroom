#pragma once

#include <DynamixelWorkbench.h>
#include "Invariants.h"
#include "../core/crstream.hpp"

constexpr uint32_t BAUDRATE = 1000000;
constexpr char DEVICE_NAME[] = "";

namespace gobin {

    namespace workbench {
        extern DynamixelWorkbench wb;      //< the actual workbench
        extern b8 initialized;   //< whether it's been initialized.
        extern uint16_t status_num;    //< current log number for logging purposes
        extern const char* cr_log;  //< contains logs from dynamixel api
    }

    namespace internal {
        // Makes sure our velocity is within the proper bounds
        inline b8 verify_vel(int32_t vel) {
            return (vel <= invar::vel_max && vel >= 0);
        }
    }

}

namespace serial {

    // sets up a serial connection
    inline void arduino_await() {
        // first we begin the connection with the base baudrate
        Serial.begin(BAUDRATE);
        while (!Serial);

        CRPrint("Connected to local machine");
    }
}