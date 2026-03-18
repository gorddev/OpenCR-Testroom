#pragma once

#include "command.h"
#include <cstdint>

/* Just a list of all the packet types that can
 * be send over serial. */

namespace gobin {

#pragma pack(push)

#pragma pack(1)

struct Position {
  uint8_t pos;
};

struct Velocity {
  uint8_t vel;
};

struct Torque {
  bool torque;
};

enum MotorMode : uint8_t { M_JOINT = 0, M_WHEEL = 1 };

struct Motor {
  uint16_t model_num;
  uint8_t id;
  uint8_t pos;
  uint8_t vel;
  MotorMode mode;
  bool torque;
  uint8_t RSRV;
};

template <uint32_t N> struct MultiMotor {
  const uint32_t num_motors = N;
  Motor motors[N];
};

struct Error {
  uint16_t error_id;
  uint8_t motor_id;
};

#pragma pack(pop)

} // namespace gobin
