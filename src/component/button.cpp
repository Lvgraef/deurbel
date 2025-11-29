#include "component/button.hpp"
#include "arduino.h"
#include "utils/utils.hpp"

component::Button::Button(const int& pin, void (*callback)()) : pin(pin), callback(callback) {
}

void component::Button::init() const {
    pinMode(pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(pin), callback, CHANGE);
}

bool component::debounce(unsigned long& lastActivation, bool& oldState, const bool& currentState) {
    if (!oldState && currentState) {
        oldState = currentState;
        if (millis() - lastActivation >= debounceTime) {
            lastActivation = millis();
            return true;
        }
        return false;
    }
    if (oldState != currentState) {
        oldState = currentState;
    }
    return false;
}
