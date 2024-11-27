#include <HX711.h>           // For load cell
#include <AccelStepper.h>    // For stepper motor control

// Pin Definitions
#define GREEN_LED 7
#define RED_LED 8
#define ORANGE_LED 9
#define BUZZER 10
#define FAN_PIN 11          // PWM pin for fan control
#define HX711_DOUT 12       // HX711 data digital pin
#define HX711_CLK 13        // HX711 clock digital pin
#define VELOCITY_SENSOR A0 // Analog pin for MPXV7002DP

// Flap control pins (example)
const int LIMIT_SWITCH_PINS[] = {2, 3, 4, 5, 6}; // Define limit switch digital pins

// Load cell and stepper motor setup
HX711 loadCell;
AccelStepper steppers[5] = {
  AccelStepper(AccelStepper::DRIVER, 22, 23), // Stepper 1
  AccelStepper(AccelStepper::DRIVER, 24, 25), // Stepper 2
  AccelStepper(AccelStepper::DRIVER, 26, 27), // Stepper 3
  AccelStepper(AccelStepper::DRIVER, 28, 29), // Stepper 4
  AccelStepper(AccelStepper::DRIVER, 30, 31) // Stepper 5
};

// Variables
float airVelocity = 0;
float dragForce = 0;
float cd = 0;

// Function prototypes
float readVelocity();
float readDragForce();
float calculateCd(float drag, float velocity);
void logData(const char* roundName);
void setStepperSpeedAndDirection(int speed, bool randomDirections);
void moveToHomePosition();
void runFanAndLogData(int targetSpeed, const char* roundName);

void setup() {
  Serial.begin(9600);

  // Initialize LEDs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(ORANGE_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Initialize HX711
  loadCell.begin(HX711_DOUT, HX711_CLK);
  if (!loadCell.is_ready()) {
    digitalWrite(RED_LED, HIGH);
    Serial.println("Load cell error!");
    while (1); // Halt
  }

  // Check velocity sensor
  if (analogRead(VELOCITY_SENSOR) == 0) {
    digitalWrite(RED_LED, HIGH);
    Serial.println("Velocity sensor error!");
    while (1); // Halt
  }

  // Initialize Stepper Motors
  for (int i = 0; i < 5; i++) {
    steppers[i].setMaxSpeed(1000);
    steppers[i].setAcceleration(500);
  }

  // Check all systems and light up green LED if ready
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(2000); // Pause for confirmation
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BUZZER, LOW);

  // Fan initial setup
  pinMode(FAN_PIN, OUTPUT);

  // Move to home position
  moveToHomePosition();
}

void loop() {
  // First Round: Flaps parallel to flow
  moveToHomePosition();
  runFanAndLogData(25, "Round 1");

  // Second Round: Flaps at 120 RPM in random directions
  analogWrite(FAN_PIN, 0); // Stop fan
  setStepperSpeedAndDirection(120, true);
  runFanAndLogData(25, "Round 2");

  // Third Round: Flaps at random speeds and directions
  analogWrite(FAN_PIN, 0); // Stop fan
  setStepperSpeedAndDirection(0, true); // Random speeds
  runFanAndLogData(25, "Round 3");

  // Return flaps to home position
  moveToHomePosition();

  // Indicate completion
  digitalWrite(ORANGE_LED, HIGH);
  while (1); // System complete, halt
}

