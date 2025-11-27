#include "button_mode/button_main.hpp"

#include <WiFi.h>
#include "component/door_button.hpp"
#include "component/potentiometer.hpp"

component::DoorButton doorButton(4);
component::Potentiometer potentiometer(39);

void button_mode::setup() {
    doorButton.init();
    potentiometer.init();
}

void button_mode::loop() {
    Serial.println(std::to_string(potentiometer.getValue()).c_str());
}