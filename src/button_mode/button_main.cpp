#include "button_mode/button_main.hpp"

#include <WiFi.h>
#include "component/door_button.hpp"
#include "component/potentiometer.hpp"

component::Potentiometer potentiometer(39);

void button_mode::setup() {
    component::createDoorButton(4);
    potentiometer.init();
}

void button_mode::loop() {
}