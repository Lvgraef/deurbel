#include <Arduino.h>
#include <thread>

#include "buzzer_mode/buzzer_main.hpp"
#include "button_mode/button_main.hpp"

bool buzzerMode = false;

void setup() {
    // Set the mode (either button or buzzer)
    pinMode(26, INPUT);
    buzzerMode = digitalRead(26);

    Serial.begin(9600);

    // Setup depending on the mode
    if (buzzerMode) {
        Serial.println("Started in buzzer mode");
        buzzer_mode::setup();
    } else {
        Serial.println("Started in button mode");
        button_mode::setup();
    }
}

void loop() {
    // Loop depending on the mode
    if (buzzerMode) {
        buzzer_mode::loop();
    } else {
        button_mode::loop();
    }
}