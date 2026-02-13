#include <SoftwareSerial.h>
SoftwareSerial BT(2, 3); // RX, TX

void setup() {
  Serial.begin(9600);   // PC monitor
  BT.begin(38400);     // AT mode baud
  Serial.println("AT MODE TERMINAL READY");
}

void loop() {
  while (Serial.available()) BT.write(Serial.read());
  while (BT.available())     Serial.write(BT.read());
}