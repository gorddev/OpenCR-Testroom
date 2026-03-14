#include "src/Keyboard.h"
#include "src/Goblin.h"

its_goblin_time();

void setup() {
  serial::arduino_await();
  goblin.init();
  goblin.motors.findMotors();

  for (auto& m : goblin.motors) {
    m.setWheelMode(30);
  }

}

void loop() {
  keys::read();

  static int32_t goal_velocity = 0;
  for (auto& m : goblin.motors) {
    if (keys::is_held('w')) {
      goal_velocity += 40;
    } else if (keys::is_held('s')) {
      goal_velocity -= 40;
    }
    m.setVelocity(goal_velocity);
  }

  keys::flush();
}