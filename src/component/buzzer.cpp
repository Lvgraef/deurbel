#include "component/buzzer.hpp"

#include <arduino.h>

component::Buzzer::Buzzer(const int &pin) : pin(pin) {
}

void component::Buzzer::init() const {
    pinMode(pin, OUTPUT);
}

void component::Buzzer::beep() const {
    digitalWrite(pin, HIGH);
    delay(200);
    digitalWrite(pin, LOW);
}
