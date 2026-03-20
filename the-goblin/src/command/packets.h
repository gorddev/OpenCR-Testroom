#pragma once

#include "command.h"
#include <cstdint>

/* Just a list of all the packet types that can
 * be send over serial. */

namespace gobin {

#pragma pack(push)

#pragma pack(1)

typedef int32_t Pos;
typedef int32_t Vel;
typedef uint8_t Torque;

enum MotorModePacket : uint8_t {
  NO_MOTOR_MODE_P = 0,
  WHEEL_MODE_P = 1,
  JOINT_MODE_P = 2
};

struct MotorPacket {
  uint16_t model_num = 0;
  uint8_t id = -1;
  MotorModePacket mode = NO_MOTOR_MODE_P;
};

template<uint16_t N = 0>
struct MotorListPacket {
  MotorPacket motors[N];

  const MotorPacket& operator[](uint16_t index) const {
    return *(reinterpret_cast<const MotorPacket*>(this)+index);
  }

  MotorPacket& operator[](uint16_t index) {
    return *(reinterpret_cast<MotorPacket*>(this)+index);
  }
};

#pragma pack(pop)

} // namespace gobin
