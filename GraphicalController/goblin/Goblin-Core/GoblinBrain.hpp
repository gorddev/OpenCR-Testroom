#pragma once
#include <cstdint>

#include "GobUpdate.hpp"
#include "MotorCore.hpp"
#include "GoblinLog.h"
#include "../the-goblin/src/command/command-io.h"
#include "serial_port.h"

/* Created by Gordie Novak on 3/15/26.
 * Purpose: 
 * Contains the core components needed to controll the goblin
 */


namespace gobin {

    class GoblinBrain {
    public:
        std::vector<MotorInterface> motors; ///< Motor interface objects interact with.
        GobUpdate updater;                  ///< Determines when we update shit.
        GoblinLog brain_log;                ///< Logs for the GoblinBrain.
        GoblinLog open_cr_log;              ///< Logs for the OpenCR board.

        explicit GoblinBrain(const char port[] = nullptr, uint64_t baudrate = 1000000);

    /* ********************************************** */

        /* -------- */
        void update(); ///< Must call every frame.

        /* ---------- MotorID ------------ */

        void setMotorID(uint8_t motor_index, uint8_t new_motor_index);

        void requestMotorScan();    ///< Requests a rescan of motors
        void queryMotors();         ///< Queries the list of motors for an update

        bool openPort(const char port[], uint64_t baudrate = 1000000);
        void closePort();          ///< Disconnects from the arduino
        void resetArduino();        ///< Performs a hard reset of the arduino
        [[nodiscard]] bool is_connected();

        const std::string& getPortPath();   ///< Returns a path to the port

        [[nodiscard]] const MotorCore& getMotorCore(u8 motor_index) const;
        void flagAllMotorsForUpdate();

        /* ---------------------------------- */
        /* ---------------------------------- */
        /* ---------------------------------- */
        /* ---------------------------------- */
        /* ---------------------------------- */
        /* ---------------------------------- */
        // Private interface below

    private:

    /* -------- Serial ----------- */

        void fetch();
        void send();

    /* ---------- Modes ------------ */

        void setWheelMode(uint8_t motor_index);
        void setJointMode(uint8_t motor_index);

    /* ---------- Velocity ------------ */

        void queryVelocity(uint8_t motor_index);
        void changeVelocity(uint8_t motor_index, int32_t d_velocity);
        void setVelocity(uint8_t motor_index, int32_t velocity);

    /* ---------- Position ------------ */

        void queryPosition(uint8_t motor_index);
        void changePosition(uint8_t motor_index, int32_t d_position);
        void setPosition(uint8_t motor_index, int32_t position);

    /* ---------- Torque ------------ */
        void setTorque(uint8_t motor_index, bool torque);

    /* ********************************************** */
        void unpack_motor_list(const Command& c);

    private: //vars
        std::vector<MotorCore> internal_motors;  ///< contains all of our motors.
        serial_port port;                       ///< The port through which we communicate with the arduino board

        explicit GoblinBrain(serial_port port)
            : port(std::move(port)) {}
    };
}
