void setup() {
    // Runs ONCE when Arduino starts
    // Like a constructor or initialization function
    Serial.begin(9600);
    pinMode(13, OUTPUT);
}

void loop() {
    // Runs FOREVER in a cycle
    // Like an infinite while loop
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
}