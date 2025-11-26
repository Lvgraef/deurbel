#include <Arduino.h>
#include "buzzer_mode/buzzer_main.hpp"
#include "button_mode/button_main.hpp"

bool buzzerMode = false;

void setup() {
    // Set the mode (either button or buzzer)
    buzzerMode = digitalRead(26);

    // Setup depending on the mode
    if (buzzerMode) {
        buzzer_mode::setup();
    } else {
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