#include "component/potentiometer.hpp"

#include <arduino.h>

component::Potentiometer::Potentiometer(const int &pin) : pin(pin) {
}

void component::Potentiometer::init() const {
    pinMode(pin, INPUT);
}

float component::Potentiometer::getValue() const {
    return analogRead(pin) / 4095.0;
}