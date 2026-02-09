// Reading analog values (0-1023 representing 0V-5V)
int sensorValue = analogRead(A0);

// Convert to voltage
float voltage = sensorValue * (5.0 / 1023.0);

// Convert to meaningful units
float temperature = (voltage - 0.5) * 100.0;  // For TMP36 sensor

// PWM pins on Uno: 3, 5, 6, 9, 10, 11
analogWrite(9, 128);  // 50% duty cycle (128/255)
analogWrite(9, 64);   // 25% duty cycle (64/255)
analogWrite(9, 255);  // 100% duty cycle (always on)


