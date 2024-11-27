
// Function to move all steppers to home position
void moveToHomePosition() {
  for (int i = 0; i < 5; i++) {
    while (digitalRead(LIMIT_SWITCH_PINS[i]) == HIGH) { // Move until the switch is triggered
      steppers[i].setSpeed(-100); // Adjust speed and direction as needed
      steppers[i].runSpeed();
    }
    steppers[i].stop(); // Stop motor when at home
  }
}




// Function to set stepper speeds and directions
void setStepperSpeedAndDirection(int speed, bool randomDirections) {
  for (int i = 0; i < 5; i++) {
    int randomSpeed = speed == 0 ? random(60, 300) : speed; // Random speed if speed == 0
    if (randomDirections && random(0, 2) == 0) {
      steppers[i].setSpeed(-randomSpeed); // CCW
    } else {
      steppers[i].setSpeed(randomSpeed); // CW
    }
  }
}

