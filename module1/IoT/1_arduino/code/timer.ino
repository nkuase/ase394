// BAD: Blocking approach
void loop() {
    digitalWrite(13, HIGH);
    delay(1000);              // Everything stops here!
    digitalWrite(13, LOW);
    delay(1000);              // And here!
}

// GOOD: Non-blocking approach
unsigned long previousMillis = 0;
const long interval = 1000;
bool ledState = false;

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        ledState = !ledState;
        digitalWrite(13, ledState);
    }

    // Other code can run here without being blocked!
    checkSensors();
    handleCommunication();
    updateDisplay();
}