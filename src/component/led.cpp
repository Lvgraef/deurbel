#include "component/led.hpp"

#include <Arduino.h>

component::Led::Led(const int &pin) : pin(pin) {
}

void component::Led::init() const {
    pinMode(pin, OUTPUT);
}

void component::Led::on() const {
    digitalWrite(pin, HIGH);
}

void component::Led::off() const {
    digitalWrite(pin, LOW);
}
