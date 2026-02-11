/**
 * Arduino Nano - AHT10 Sensor Reader
 * 
 * Reads temperature and humidity from AHT10 sensor
 * and sends data to ESP32C6 via Serial communication
 * 
 * Hardware Connections:
 * AHT10:
 *   VCC -> 5V
 *   GND -> GND
 *   SDA -> A4
 *   SCL -> A5
 * 
 * To ESP32C6 (via Level Shifter):
 *   TX (D1) -> Level Shifter HV1 -> LV1 -> ESP32C6 RX (GPIO17)
 *   RX (D0) -> Level Shifter HV2 -> LV2 -> ESP32C6 TX (GPIO16)
 */

#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

void setup() {
  // Serial for ESP32C6 communication
  Serial.begin(9600);
  
  // Initialize AHT10 sensor
  if (!aht.begin()) {
    // Send error message to ESP32C6
    Serial.println("ERROR:AHT10_NOT_FOUND");
    while (1) {
      delay(1000);
    }
  }
  
  // Wait a bit for sensor to stabilize
  delay(100);
}

void loop() {
  // Read sensor data
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  
  // Send data in JSON-like format for easy parsing
  // Format: TEMP:23.45,HUMID:55.32
  Serial.print("TEMP:");
  Serial.print(temp.temperature, 2);
  Serial.print(",HUMID:");
  Serial.println(humidity.relative_humidity, 2);
  
  // Send data every 2 seconds
  delay(2000);
}