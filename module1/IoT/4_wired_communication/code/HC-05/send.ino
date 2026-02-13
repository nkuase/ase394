# include <SoftwareSerial.h>

SoftwareSerial BT(2, 3);

void setup() {
  Serial.begin(9600); // To PC
  BT.begin(9600); // To other HC-05 (Data mode baud)
}

void loop() {
  BT.println("Hello from Arduino A");
  delay(1000);
}