#include <AccelStepper.h>
#include "HX711.h"

// Motor control pins
const int dirPins[] = {3, 5, 7, 9, 11};
const int stepPins[] = {2, 4, 6, 8, 10};
#define NUM_MOTORS 5

// Limit switch pins
const int limitSwitchPins[] = {22, 24, 26, 28, 30};


// Load cell pins
const uint8_t dataPin = 25;
const uint8_t clockPin = 23;
HX711 scale;
float loadcell_cal_factor = 65;


// Pitot tube parameters
float V_0 = 5.04;
float rho = 1.225;
float calibration_factor = 0.5;
float offset = 0;
int offset_size = 10;
int veloc_mean_size = 20;
int zero_span = 2;


// Frontal area of the object (adjust based on your setup, in m²)
float frontal_area = 0.019165;  // Example: 0.01 m²


// Motor array
AccelStepper steppers[NUM_MOTORS] = {
  AccelStepper(AccelStepper::DRIVER, stepPins[0], dirPins[0]),
  AccelStepper(AccelStepper::DRIVER, stepPins[1], dirPins[1]),
  AccelStepper(AccelStepper::DRIVER, stepPins[2], dirPins[2]),
  AccelStepper(AccelStepper::DRIVER, stepPins[3], dirPins[3]),
  AccelStepper(AccelStepper::DRIVER, stepPins[4], dirPins[4])
};


void alignMotors();
void runMotorsForInterval(bool randomSpeed, bool randomDirection, unsigned long interval);
float measureVelocity();
void measureAndPrintValues();

void setup() {
  Serial.begin(115200);

  // Initialize load cell
  scale.begin(dataPin, clockPin);
  scale.set_scale(loadcell_cal_factor);  // Calibration factor
  scale.tare();

  // Initialize pitot tube offset
  for (int ii = 0; ii < offset_size; ii++) {
    offset += analogRead(A1) - (1023 / 2);
  }
  offset /= offset_size;

  // Initialize motors
  for (int i = 0; i < NUM_MOTORS; i++) {
    steppers[i].setMaxSpeed(400);
    steppers[i].setAcceleration(250);
  }

  Serial.println("System initialized.");
}

void loop() {
  delay(5000);
 // Handle tare command for load cell
  if (Serial.available()) {
    char command = Serial.read();
    if (command == 't') {
      scale.tare();  // Reset the scale to zero
      Serial.println("Scale tared.");
    }
  }

  // Round 1: Align motors and measure values for 10 seconds
  Serial.println("Round 1");
  alignMotors();  // Align motors
  unsigned long roundStartTime = millis();  // Record the start time
  unsigned long roundDuration = 1000;  // Duration for Round 1 (same as other rounds)

  // Keep measuring and printing values for the duration of this round
  while (millis() - roundStartTime < roundDuration) {
    measureAndPrintValues();  // Measure and print values
    delay(100);  // Wait for 1 second before next measurement (to avoid flooding the serial monitor)
  }
  delay(2000);  // Short delay after Round 1
  
  // // Round 2: Constant speed, same direction
  // Serial.println("Round 2");
  // runMotorsForInterval(false, false, 10000);
  // delay(2000);

  // //Round 3: Constant speed, random direction
  // Serial.println("Round 3");
  // runMotorsForInterval(false, true, 10000);
  // delay(2000);

  // // Round 4: Random speed and direction
  // Serial.println("Round 4");
  // runMotorsForInterval(true, true, 10000);
  // delay(2000);

     Serial.print("All Processes complete!!!! ");
   //while (1);  // End the program
}
