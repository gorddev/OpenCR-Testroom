#include "src/Keyboard.h"
#include "src/Goblin.h"

its_goblin_time(goblin);


void setup() {
  serial::arduino_await();
  goblin.init();

  for (auto& m : goblin.motors) {
    m.setWheelMode(30);
  }
}

void loop() {
  keys::read();

  for (auto& m : goblin.motors) {
    if (keys::is_held('w')) {
      m.changeVelocity(40);
    } else if (keys::is_held('s')) {
      m.changeVelocity(-40);
    }
  }

  keys::flush();
}