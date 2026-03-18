#define OPEN_CR
#include "src/include/Keyboard.h"
#include "src/include/Goblin.h"
#include "../serialization/command.h"


its_goblin_time(goblin);


void setup() {
  serial::arduino_await();
  goblin.init();

  for (auto& m : goblin.motors) {
    m.setWheelMode(30);
  }
}

constexpr uint16_t max_buf = 1000;
uint8_t serial_buf[max_buf];

void loop() {

  int bytes = Serial.available();
  // if we have bytes for reading.
  if (bytes > max_buf) {

  }
  if (bytes >= 0) {
    uint8_t buf[bytes]; //<
    const size_t read = Serial.readBytes(buf, bytes);

    if (read == 0) {

    }
  }

  for (auto& m : goblin.motors) {
    if (keys::is_held('w')) {
      m.changeVelocity(40);
    } else if (keys::is_held('s')) {
      m.changeVelocity(-40);
    }
  }

  keys::flush();
}