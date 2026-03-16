#pragma once

#include "Motor.h"
#include "Workbench.h"

//#define CR_RELEASE_BUILD

namespace dxl {

    // Keeps track of the various components of the workbench.
    class Goblin {
    public:
        DynamixelWorkbench& wb;
        bool& initialized;
        const char*& log;
        const uint16_t& status_num;

        MotorList motors{};

        bool init() {

            if (initialized) {
                CR_PANIC("dxl_workbench already initialized.");
                CR_EXIT;
            }

            if (!wb.init(DEVICE_NAME, BAUDRATE, &log)) {
                CR_PANIC("Failed to initialize workbench. Received error:\n\t")
                Serial.println(log);
                CR_EXIT;
            }

            workbench::success("Connected to board.");
            initialized = true;

            motors.findMotors();

            return true;
        }

        Goblin(DynamixelWorkbench& wb, bool& init, uint16_t& status_num, const char*& log)
            : wb(wb), initialized(init), log(log), status_num(status_num) {}
    };

}

namespace serial {

    // sets up a serial connection
    void arduino_await() {
        // first we begin the connection with the base baudrate
        Serial.begin(BAUDRATE);
        while (!Serial);

        Serial.print("Connected to local machine.\n");
    }
}

#define its_goblin_time(goblin_var_name) \
    dxl::Goblin goblin_var_name(dxl::workbench::wb, dxl::workbench::initialized, dxl::workbench::status_num, dxl::workbench::cr_log);



