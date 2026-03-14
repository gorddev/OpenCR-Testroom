#include <DynamixelWorkbench.h>
/* Contains all the macros for working with our OpenCR board */

#define BAUDRATE  1000000
#define DEVICE_NAME "" 

namespace cr {

  DynamixelWorkbench dxl;

  struct dxlStatusStruct {
    const char* log;
    uint32_t baudrate = 0;
    bool initialized = false;
  } dxl_status;

  // sets up a serial connection
  void setup_serial(uint32_t baudrate) {
    // first we begin the connection with the base baudrate
    Serial.begin(baudrate);
    while(!Serial) //< wait for the serial connection
    Serial.print("Connected to local machine.\n");
    dxl_status.baudrate = baudrate;
  }

  // initialize the dxl workbench without a baudrate
  void init_dxl_workbench() {
    if (dxl_status.baudrate == 0) {
      Serial.print("!!Panic!! Baudrate not set for system");
      while(1);
    }
    bool result = false;

    result = dxl.init(DEVICE_NAME, dxl_status.baudrate, &dxl_status.log);
    if (result == false) {
      Serial.print("ERROR: Failed to init board: ");
      Serial.println(dxl_status.log); // print error message
      while(1); // Halt
    }

    Serial.println("Connected to board. ");
    dxl_status.initialized = true;
  }

  // initializes the dxl_workbench with a baudrate
  void init_dxl_workbench_b(uint32_t baudrate) {
    dxl_status.baudrate = baudrate;
    cr::init_dxl_workbench();
  }

  // initializes a motor with the dxl_workbench
  void init_motor(uint32_t motor_id) {

    if (!dxl_status.initialized) {
      Serial.print("!!Panic!! dxl_workbench not initialized. Please call cr::init_dxl_workbench()");
    }

    uint16_t model_number = 0;
    bool result = dxl.ping(motor_id, &model_number, &dxl_status.log);
    if (result == false) {
      Serial.print("!!Panic!! Failed to find motor ID ");
      Serial.println(motor_id);
      return;
    }
    Serial.print("Found Motor. ID: ");
    Serial.print(motor_id);
    Serial.print(" Model: ");
    Serial.println(model_number);

    result = dxl.jointMode(motor_id, 0, 0, &dxl_status.log);
    if (result == false) {
      Serial.print("!!Panic!! Failed to set joint mode for motor ");
      Serial.println(motor_id);
      return;
    }
    Serial.print("Connected to motor & joint mode set.\n");
  }
}


namespace keyboard {

  struct Keys {
    bool k_map[26];

    // updates the given character to be true. 
    void set(char c) {
      if (c >= 'a' && c <= 'z') {
        k_map[c - 'a'] = true;
      }
    }
    // returns true if the key is held down
    bool isDown(char c) {
      if (c >= 'a' && c <= 'z') {
        return k_map[c - 'a'];
      }
      return false;
    }
    // flushes all keys to false
    void flush() {
      for (int i = 0; i < 26; i++) {
        k_map[i] = false;
      }
    }
  } keys;

  // updates the keyboard with any given keypresses.
  void get_input() {
    keys.flush();

    if (Serial.available() > 0) {
      char incomingByte = Serial.read(); // Read the character

      Serial.print("Got character: ");
      Serial.println(incomingByte);

      if (incomingByte == 'W' || incomingByte == 'w') {
        keys.set('w');
      } 
      else if (incomingByte == 'S' || incomingByte == 's') {
        keys.set('s');
      }
    }
  }

  void control(uint32_t motor_id) {
    // Check if a key was pressed on the laptop
    if (keys.isDown('w')) {
      Serial.println("Moving forward");
      cr::dxl.goalPosition(motor_id, (int32_t)2400);
    } else if (keys.isDown('s')) {
      cr::dxl.goalPosition(motor_id, (int32_t)0);
    }   
  }
}