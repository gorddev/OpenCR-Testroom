#define OPEN_CR

#include "src/command/fetch.h"
#include "src/core/crconnect.h"
#include "music.h"

its_goblin_time(goblin);

using namespace gobin;

void setup() {
  music::mary_had_lamb_meat();
  // first we wait for a serial connection.
  serial::arduino_connect();
  // initialize the goblin
  goblin.init();

  for (auto& m : goblin.motors) {
    m.setJointMode(invar::vel_max, invar::acc_rate);
  }
}


void loop() {

  if (!serial::arduino_ping()) {
    serial::arduino_await_reconnect(goblin);
  }

  delay(20);

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
    case T_MOTOR_JOINT_MODE: //< switch a motor to joint mode
      fetch::joint_mode(goblin, c);
      break;
    case T_MOTOR_WHEEL_MODE: //< switch a motor to wheel mode
      fetch::wheel_mode(goblin, c);
      break;
    case T_MOTOR_TORQUE:  //< disable/enable the torque on a motor
      fetch::torque(goblin, c);
      break;
    case T_RESET: //< reset the arduino board.
      fetch::reset(goblin);
      break;
    default:
      CRError(UNKNOWN_COMMAND_TYPE, c.type);
      break;
    }

    goblin.port.pop();
  }
}
