


// Function to log data

// Function to run fan and log data
void runFanAndLogData(int targetSpeed, const char* roundName) {
  airVelocity = 0;
  while (airVelocity < targetSpeed) {
    analogWrite(FAN_PIN, analogRead(FAN_PIN) + 5); // Gradual speed increase
    airVelocity = readVelocity();
    dragForce = readDragForce();
    cd = calculateCd(dragForce, airVelocity);
    logData(roundName);
    delay(1000); // Log data every second
  }
}


// Function to read velocity from pitot tube
float readVelocity() {
  float raw = analogRead(VELOCITY_SENSOR);
  float pressure = (raw / 1023.0) * 5.0; // Convert to voltage
  return sqrt(2 * pressure / 1.225); // Using Bernoulli's principle
}

// Function to read drag force from load cell
float readDragForce() {
  return loadCell.get_units(10); // Convert raw data to force
}

// Function to calculate drag coefficient
float calculateCd(float drag, float velocity) {
  float area = 0.1; // Parachute cross-sectional area in m² (adjust as needed)
  return (2 * drag) / (1.225 * area * pow(velocity, 2));
}


void logData(const char* roundName) {
  Serial.print(roundName);
  Serial.print(",");
  Serial.print(millis());
  Serial.print(",");
  Serial.print(airVelocity);
  Serial.print(",");
  Serial.print(dragForce);
  Serial.print(",");
  Serial.println(cd);
}
