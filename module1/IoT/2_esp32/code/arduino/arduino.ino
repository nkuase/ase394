/**
 * Arduino Nano - AHT10 Sensor Reader (SoftwareSerial Version)
 * 
 * Uses SoftwareSerial to avoid conflict with USB Serial (D0/D1)
 * Reads temperature and humidity from AHT10 sensor
 * and sends data to ESP32C6 via Software Serial pins D2/D3
 * 
 * Hardware Connections:
 * AHT10:
 *   VCC -> 5V
 *   GND -> GND
 *   SDA -> A4
 *   SCL -> A5
 * 
 * To ESP32C6 (via Level Shifter):
 *   D2 (SW TX) -> Level Shifter HV1 -> LV1 -> ESP32C6 RX (GPIO17)
 *   D3 (SW RX) -> Level Shifter HV2 -> LV2 -> ESP32C6 TX (GPIO16)
 *   5V -> Level Shifter HV
 *   GND -> Level Shifter GND (and ESP32C6 GND)
 * 
 * IMPORTANT: D0/D1 are now FREE for USB communication!
 */

#include <Adafruit_AHTX0.h>
#include <SoftwareSerial.h>

// Software Serial pins for ESP32C6 communication
// D2 = TX (to ESP32C6 RX)
// D3 = RX (from ESP32C6 TX)
SoftwareSerial esp32Serial(3, 2); // RX, TX

Adafruit_AHTX0 aht;

void setup() {
  // Hardware Serial for USB debugging (can use Serial Monitor!)
  Serial.begin(9600);
  Serial.println("Arduino Nano - AHT10 to ESP32C6");
  Serial.println("=================================");
  
  // Software Serial for ESP32C6 communication
  esp32Serial.begin(9600);
  
  // Initialize AHT10 sensor
  Serial.print("Initializing AHT10 sensor... ");
  if (!aht.begin()) {
    Serial.println("FAILED!");
    // Send error to both USB and ESP32C6
    Serial.println("ERROR:AHT10_NOT_FOUND");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("OK!");
  
  // Wait for sensor to stabilize
  delay(100);
  Serial.println("Starting data transmission...");
}

void loop() {
  // Read sensor data
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  
  // Format data string
  String dataString = "TEMP:";
  dataString += String(temp.temperature, 2);
  dataString += ",HUMID:";
  dataString += String(humidity.relative_humidity, 2);
  
  // Send to ESP32C6 via SoftwareSerial
  esp32Serial.println(dataString);
  
  // Also print to USB Serial Monitor for debugging
  Serial.print("Sent to ESP32C6: ");
  Serial.println(dataString);
  Serial.print("  Temperature: ");
  Serial.print(temp.temperature, 2);
  Serial.println(" °C");
  Serial.print("  Humidity: ");
  Serial.print(humidity.relative_humidity, 2);
  Serial.println(" %");
  Serial.println("-------------------");
  
  // Send data every 2 seconds
  delay(2000);
}
