

#include "HX711.h"

HX711 scale;

// Adjust pins if needed
uint8_t dataPin = 25;
uint8_t clockPin = 23;

float f;

void setup() {
  Serial.begin(115200);
  scale.begin(dataPin, clockPin);

  // Calibration factor (adjust based on your calibration process)
  float scale_factor = 0.001;
  float cal_factor = scale_factor*2900.7;
  // float cal_factor = 92.546;
  scale.set_scale(cal_factor);
  scale.tare();  // Reset the scale to zero
  Serial.println("Scale initialized. Type 't' to tare.");
}

void loop() {
  // Check for serial input to handle tare
  if (Serial.available()) {
    char command = Serial.read();
    if (command == '\n') {
      scale.tare();  // Reset the scale to zero
      Serial.println("Scale tared.");
    }
  }

  // Read and print weight continuously
  f = scale.get_units(5);
  Serial.print("Weight: ");
  Serial.println(f);
  delay(250);
}

