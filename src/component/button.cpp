#include "component/button.hpp"
#include "arduino.h"

#define debounce_time 10

int counter = 0;

component::Button::Button(const int& pin) : pin(pin) {
}

void component::Button::init() const {
    pinMode(pin, INPUT);
}

void component::Button::update() {
    const bool input = digitalRead(pin);
    if (millis() - lastActivation < 10) {
        return;
    }
    if (state) {
        if (!input) {
            state = false;
        }
    } else {
        if (input) {
            lastActivation = millis();
            state = true;
            callback();
        }
    }
}