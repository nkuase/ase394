const int BUTTON_PIN = 2;
const int LED_PIN = 13;

void setup() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    bool isPressed = !digitalRead(BUTTON_PIN);  // Inverted due to pull-up

    digitalWrite(LED_PIN, isPressed);

    Serial.print("Button: ");
    Serial.println(isPressed ? "PRESSED" : "RELEASED");

    delay(100);  // Debouncing delay
}