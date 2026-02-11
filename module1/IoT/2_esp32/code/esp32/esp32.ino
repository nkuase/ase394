/**
 * ESP32C6 - Web Server for Sensor Data
 * 
 * Receives sensor data from Arduino Nano via Serial
 * and serves it via web server
 * 
 * Hardware Connections (via Level Shifter):
 *   RX (GPIO17) <- LV1 <- HV1 <- Arduino TX (D1)
 *   TX (GPIO16) -> LV2 -> HV2 -> Arduino RX (D0)
 *   3.3V -> Level Shifter LV
 *   GND -> Level Shifter GND
 */

#include <WiFi.h>
#include <WebServer.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Web server on port 80
WebServer server(80);

// Serial pins for ESP32C6
#define RXD2 17  // Connect to Arduino TX via level shifter
#define TXD2 16  // Connect to Arduino RX via level shifter

// Sensor data variables
float temperature = 0.0;
float humidity = 0.0;
String lastUpdate = "No data yet";
bool dataValid = false;

void setup() {
  // Serial for debugging
  Serial.begin(115200);
  
  // Serial for Arduino communication
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
  Serial.println("\nESP32C6 Sensor Web Server");
  Serial.println("=========================");
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Setup web server routes
  server.on("/data", handleData);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Handle web server requests
  server.handleClient();
  
  // Read data from Arduino
  if (Serial1.available()) {
    String data = Serial1.readStringUntil('\n');
    data.trim();
    
    Serial.println("Received: " + data);
    
    // Parse data: TEMP:23.45,HUMID:55.32
    if (data.startsWith("TEMP:")) {
      parseData(data);
      dataValid = true;
      lastUpdate = getTimeString();
    } else if (data.startsWith("ERROR:")) {
      dataValid = false;
      lastUpdate = data;
    }
  }
}

void parseData(String data) {
  // Parse: TEMP:23.45,HUMID:55.32
  int tempIndex = data.indexOf("TEMP:");
  int humidIndex = data.indexOf(",HUMID:");
  
  if (tempIndex >= 0 && humidIndex > tempIndex) {
    String tempStr = data.substring(tempIndex + 5, humidIndex);
    String humidStr = data.substring(humidIndex + 7);
    
    temperature = tempStr.toFloat();
    humidity = humidStr.toFloat();
  }
}

String getTimeString() {
  unsigned long seconds = millis() / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  
  seconds = seconds % 60;
  minutes = minutes % 60;
  
  char timeStr[20];
  sprintf(timeStr, "%02lu:%02lu:%02lu ago", hours, minutes, seconds);
  return String(timeStr);
}

// JSON data endpoint
void handleData() {
  String json = "{";
  json += "\"temperature\":" + String(temperature, 2) + ",";
  json += "\"humidity\":" + String(humidity, 2) + ",";
  json += "\"valid\":" + String(dataValid ? "true" : "false") + ",";
  json += "\"lastUpdate\":\"" + lastUpdate + "\"";
  json += "}";
  
  server.send(200, "application/json", json);
}
