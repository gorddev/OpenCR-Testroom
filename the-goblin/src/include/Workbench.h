#pragma once

#include <DynamixelWorkbench.h>
#include "../serialization/command.h"
#include "Invariants.h"
#include "CRError.hpp"

#define BAUDRATE  1000000
#define DEVICE_NAME ""

// printing & error macros
#define CR_PRINT(msg) \
    auto data = gobin::bitcast(msg); \
    Serial.write(data.bits, data.size());

#define CR_STATUS(s, msg) Serial.print("["); Serial.print(s); Serial.print("]\t"); Serial.print(msg);
#define CR_PANIC(msg) Serial.print("\n----------> ["); Serial.print(__func__); Serial.print("()]\n!!Panic!! : " msg);
#define CR_EXIT Serial.print("\n-----------\n~exit~ ...\n"); while(true)

namespace dxl {

    namespace workbench {
        extern DynamixelWorkbench wb;      //< the actual workbench
        extern bool initialized;   //< whether it's been initialized.
        extern uint16_t status_num;    //< current log number for logging purposes
        extern const char* cr_log;  //< contains logs from dynamixel api

        /// Status printout for the dxl workbench
        inline void status(const char msg[], const bool newline = true) {
            CR_STATUS(status_num, msg);
            if (newline) { CR_PRINT("\n"); }
        }
        /// Success printout for the dxl workbench.
        inline void success(const char msg[], const bool newline = true) {
            CR_STATUS(status_num++, msg);
            if (newline) { CR_PRINT("\n"); }
        }
    }

    namespace internal {
        // Makes sure our velocity is within the proper bounds
        inline bool verify_vel(int32_t vel) {
            return (vel <= invar::vel_max && vel >= 0);
        }
    }

}
