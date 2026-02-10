void setup() {
    Serial.begin(9600);  // Baud rate: bits per second
}

void loop() {
    // Sending data
    Serial.println("Hello Computer!");
    Serial.print("Sensor value: ");
    Serial.println(analogRead(A0));

    // Receiving data
    if (Serial.available() > 0) {
        String command = Serial.readString();
        command.trim();  // Remove whitespace

        if (command == "LED_ON") {
            digitalWrite(13, HIGH);
            Serial.println("LED turned ON");
        } else if (command == "LED_OFF") {
            digitalWrite(13, LOW);
            Serial.println("LED turned OFF");
        }
    }

    delay(1000);
}