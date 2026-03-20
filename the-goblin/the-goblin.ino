#define OPEN_CR

#include "src/command/fetch.h"
#include "src/core/crconnect.h"

its_goblin_time(goblin);

using namespace gobin;

void setup() {
  // first we wait for a serial connection.
  serial::arduino_connect();
  // initialize the goblin
  goblin.init();

  // set all fo them to wheel mode
  for (auto& m : goblin.motors) {
    m.setWheelMode(30);
  }
}


void loop() {

  if (!serial::arduino_ping()) {
    serial::arduino_await_reconnect(goblin);
  }

  while (goblin.port.fetch()) {
    Command c = goblin.port.command();

    switch (c.target) {
    case T_PRINT_CONSOLE: // if the serial wants to echo something.
      fetch::echo_console(goblin, c);
      break;
    case T_MOTOR_VELOCITY: // if the serial commands for velocity-related operations
      fetch::motor_velocity(goblin, c);
      break;
    case T_MOTOR_POSITION: //< if the commands for position-related operations
      fetch::motor_position(goblin, c);
      break;
    case T_MOTOR_LIST: //< if the serial asks for a list of all the current motors
      fetch::motor_list(goblin, c);
      break;
    case T_MOTOR_ID: //< if the serial requests to change a motor id.
      fetch::motor_id(goblin, c);
      break;
    case T_MOTOR_JOINT_MODE:
      fetch::joint_mode(goblin, c);
      break;
    case T_MOTOR_WHEEL_MODE:
      fetch::wheel_mode(goblin, c);
      break;
    case T_MOTOR_TORQUE:
      fetch::torque(goblin, c);
      break;
    default:
      CRError(UNKNOWN_COMMAND_TYPE, c.type);
      break;
    }

    goblin.port.pop();
  }
}
