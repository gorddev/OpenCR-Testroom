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

  for (auto& m : goblin.motors) {
    if (keys::is_held('w')) {
      m.setVelocity(40);
    } else if (keys::is_held('s')) {
      m.setVelocity(-40);
    } else {
      m.setVelocity(0);
    }
  }
}