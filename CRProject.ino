#include "OpenCRMacros.h"

void setup() {
  cr::setup_serial(BAUDRATE);
  cr::init_dxl_workbench();
  //cr::init_motor(4);
  cr::init_motor(3);
}

void loop() {
  keyboard::get_input();
  keyboard::control(3);
}