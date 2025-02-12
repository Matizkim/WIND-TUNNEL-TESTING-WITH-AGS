void runMotorsForInterval(bool randomSpeed, bool randomDirection, unsigned long interval) {
  unsigned long startTime = millis();
  unsigned long lastMeasurementTime = 0;

  // Configure motors
  for (int i = 0; i < NUM_MOTORS; i++) {
    if (randomSpeed) {
      steppers[i].setMaxSpeed(random(100, 501));
    } else {
      steppers[i].setMaxSpeed(500);
    }
    int direction = randomDirection ? (random(0, 2) == 0 ? 1 : -1) : 1;
    steppers[i].setSpeed(direction * steppers[i].maxSpeed());
  }

  // Run motors and take measurements
  while (millis() - startTime < interval) {
    for (int i = 0; i < NUM_MOTORS; i++) {
      steppers[i].runSpeed(); // Run motors at the configured speed
    }

    // Take measurements every 1 second
    if (millis() - lastMeasurementTime >= 100) {
      measureAndPrintValues();
      lastMeasurementTime = millis();
    }
  }

  // Stop all motors
//   for (int i = 0; i < NUM_MOTORS; i++) {
//     steppers[i].stop();
//   }
 }
