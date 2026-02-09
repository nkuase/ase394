class LED {
private:
    int pin;
    bool state;
    unsigned long lastToggleTime;
    unsigned long blinkInterval;

public:
    LED(int pinNumber) : pin(pinNumber), state(false),
        lastToggleTime(0), blinkInterval(1000) {
        pinMode(pin, OUTPUT);
    }

    void on() {
        state = true; digitalWrite(pin, HIGH);
    }

    void off() {
        state = false; digitalWrite(pin, LOW);
    }

    void toggle() {
        state = !state; digitalWrite(pin, state);
    }

    void setBlinkInterval(unsigned long interval) {
        blinkInterval = interval;
    }

    void update() {  // Call this in loop()
        unsigned long currentTime = millis();
        if (currentTime - lastToggleTime >= blinkInterval) {
            toggle();
            lastToggleTime = currentTime;
        }
    }
};

// Usage
LED statusLED(13);
LED warningLED(12);

void setup() {
    statusLED.setBlinkInterval(500);   // Fast blink
    warningLED.setBlinkInterval(2000); // Slow blink
}

void loop() {
    statusLED.update();
    warningLED.update();
}