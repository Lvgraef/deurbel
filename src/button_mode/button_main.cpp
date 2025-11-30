#include "button_mode/button_main.hpp"

#include <LiquidCrystal_I2C.h>
#include "component/bell_button.hpp"
#include "component/door_button.hpp"
#include "component/potentiometer.hpp"
#include "component/ultrasone_sensor.hpp"

component::Potentiometer potentiometer(39);
component::Potentiometer potentiometer2(14);
component::UltrasoneSensor<12, 13> ultrasoneSensor();
component::UltrasoneSensor<26,25> ultrasoneSensor2();
component::DoorButton<4> doorButton();
component::BellButton<36> bellButton();
LiquidCrystal_I2C display(0x20, 16, 2);

void button_mode::setup() {
    potentiometer.init();
}

void button_mode::loop() {
}