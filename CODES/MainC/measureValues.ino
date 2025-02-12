float measureVelocity() {
  float adc_avg = 0;
  for (int ii = 0; ii < veloc_mean_size; ii++) {
    adc_avg += analogRead(A1) - offset;
  }
  adc_avg /= veloc_mean_size;

  float veloc = 0.0;
  if (adc_avg > 512 - zero_span && adc_avg < 512 + zero_span) {
    veloc = 0.0;
  } else if (adc_avg < 512) {
    veloc = -sqrt((-10000.0 * ((adc_avg / 1023.0) - 0.5)) / rho);
  } else {
    veloc = sqrt((10000.0 * ((adc_avg / 1023.0) - 0.5)) / rho);
  }

  return abs(veloc * calibration_factor);
}

void measureAndPrintValues() {
  float weight = scale.get_units(5);
  float velocity = measureVelocity();
  float force = weight * 9.81 / 1000; // Simplified drag force calculation
  float drag_coefficient = force / (0.5 * rho * frontal_area * velocity * velocity); // Simplified Cd formula

  // Print results to Serial Monitor
  Serial.print("Weight: ");
  Serial.print(weight);
  Serial.print(" g,     Drag Force: ");
  Serial.print(force);
  Serial.print(" N,     Velocity: ");
  Serial.print(velocity);
  Serial.print(" m/s,     Drag Coefficient: ");
  Serial.println(drag_coefficient);
}
