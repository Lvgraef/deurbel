#include "component/button.hpp"
#include "arduino.h"

component::Button::Button(const int& pin, void (*callback)()) : pin(pin), callback(callback) {
}

void component::Button::init() const {
    pinMode(pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(pin), callback, RISING);
}

bool component::cooldown(unsigned long &lastActivation) {
    if (millis() - lastActivation >= cooldownTime) {
        lastActivation = millis();
        return true;
    }
    return false;
}
