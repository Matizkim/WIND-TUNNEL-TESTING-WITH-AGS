



void alignMotors() {
  Serial.println("Aligning motors...");
  
  // // Initialize limit switch pins as inputs
  // for (int i = 0; i < NUM_MOTORS; i++) {
  //   pinMode(limitSwitchPins[i], INPUT_PULLUP);  // Set limit switch pins as input with internal pull-up
  // }

  // // Move all motors towards the limit switches simultaneously
  // for (int i = 0; i < NUM_MOTORS; i++) {
  //   steppers[i].moveTo(-1000); // Move to a position that will trigger the limit switch
  // }

  // // Continue running the motors until all limit switches are triggered
  // bool allAligned = false;
  // while (!allAligned) {
  //   allAligned = true;
  //   for (int i = 0; i < NUM_MOTORS; i++) {
  //     steppers[i].run(); // Run all motors at the same time
  //     if (digitalRead(limitSwitchPins[i]) == HIGH) { // If limit switch is not pressed
  //       allAligned = false; // Keep checking until all motors reach their limit switch
  //     }
  //   }
  // }

  // // Once all motors are aligned, stop them
  // for (int i = 0; i < NUM_MOTORS; i++) {
  //   steppers[i].stop();
  // }

  Serial.println("Motors aligned.");
}
