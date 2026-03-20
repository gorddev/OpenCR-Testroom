#pragma once

#include "command.h"
#include <cstdint>

/* Just a list of all the packet types that can
 * be send over serial. */

namespace gobin {

#pragma pack(push)

#pragma pack(1) //< pack everything

typedef int32_t Pos;
typedef int32_t Vel;
typedef uint8_t Torque;

enum MotorModePacket : uint8_t {
  NO_MOTOR_MODE_P = 0,
  WHEEL_MODE_P = 1,
  JOINT_MODE_P = 2
};

struct MotorPacket {
  union {i32 pos, vel; };
  i16 model_num = 0;
  i8 id = -1;
  MotorModePacket mode = NO_MOTOR_MODE_P;
  float radians{};
};

#pragma pack(pop)

} // namespace gobin
