#pragma once
#include "../goblin/Goblin.h"
#include "crstream.h"

namespace serial {
    // sets up a serial connection
    inline void arduino_connect() {
        // first we begin the connection with the base baudrate
        Serial.begin(BAUDRATE);

        while (!Serial);

        CRPrint("--Serial Connected--");
    }

    inline bool arduino_ping() {
        static u32 i = 0;
        if (!Serial) return false;
        if (i++ >= BAUDRATE*10) {
            CRPrint("--Ping: Process Alive--");
            i = 0;
        } return true;
    }

    inline void arduino_await_reconnect(gobin::Goblin& g) {
        for (auto& m : g.motors)
            m.disableTorque();
        while (!Serial);
        CRPrint("--Serial Reconnected--");
        for (auto& m : g.motors)
            m.enableTorque();
        g.motors.sendSerializedMotors(true);

    }
}
