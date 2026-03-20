#pragma once

#include "Motor.h"
#include "Workbench.h"
#include "../core/crdef.h"
#include "../command/serial_receiver.h"

//#define CR_RELEASE_BUILD

namespace gobin {

    // Keeps track of the various components of the workbench.
    class Goblin {
    public:
        MotorList motors{};
        serial_receiver port;

        b8 init() {

            if (initialized) {
                CRPanic("dxl_workbench already initialized.");
            }

            if (!wb.init(DEVICE_NAME, BAUDRATE, &log)) {
                CRPrint("Failed to initialize workbench. Received error:\n\t");
                CRPrint(log, strlen(log));
                CRExit();
            }

            CRPrint("Connected to board");
            initialized = true;

            motors.findMotors();

            return true;
        }

        Goblin(DynamixelWorkbench& wb, b8& init, u16& status_num, const char*& log)
            : wb(wb), initialized(init), log(log), status_num(status_num) {}

    public:
        DynamixelWorkbench& wb;
        b8& initialized;
        const char*& log;
        const u16& status_num;
    };

}

#define its_goblin_time(goblin_var_name) \
    gobin::Goblin goblin_var_name(gobin::workbench::wb, gobin::workbench::initialized, gobin::workbench::status_num, gobin::workbench::cr_log);



