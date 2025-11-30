#include "button_mode/button_main.hpp"

#include <LiquidCrystal_I2C.h>
#include "component/bell_button.hpp"
#include "component/door_button.hpp"
#include "component/potentiometer.hpp"
#include "component/ultrasone_sensor.hpp"

component::Potentiometer potentiometer(39);
component::Potentiometer potentiometer2(14);
component::UltrasoneSensor<12, 13> ultrasoneSensor;
component::UltrasoneSensor<26,25> ultrasoneSensor2;
component::DoorButton<4> doorButton();
component::BellButton<36> bellButton();
LiquidCrystal_I2C display(0x20, 16, 2);

constexpr int sensorChangeFilterTime = 500;

bool sensor1State = false;
unsigned long lastChangedSensor1 = 0;
bool sensor2State = false;
unsigned long lastChangedSensor2 = 0;

void ultrasoneCallback() {

}

void updateUltrasoneSensors() {
    ultrasoneSensor.update();
    ultrasoneSensor2.update();
    const int16_t distanceInput = utils::getDistanceInput(potentiometer.getValue());
    const int16_t distance2Input = utils::getDistanceInput(potentiometer2.getValue());
    if (ultrasoneSensor.getDistance() < distanceInput) {
        if (!sensor1State) {
            if (millis() - lastChangedSensor1 > sensorChangeFilterTime) {
                sensor1State = true;
                utils::selectedClient = 0;
                ultrasoneCallback();
                lastChangedSensor1 = millis();
            }
        }
    } else {
        if (sensor1State) {
            if (millis() - lastChangedSensor1 > sensorChangeFilterTime) {
                sensor1State = false;
                ultrasoneCallback();
                lastChangedSensor1 = millis();
            }
        }
    }
    if (ultrasoneSensor2.getDistance() < distance2Input) {
        if (!sensor2State) {
            if (millis() - lastChangedSensor2 > sensorChangeFilterTime) {
                sensor2State = true;
                utils::selectedClient = 1;
                ultrasoneCallback();
                lastChangedSensor2 = millis();
            }
        }
    } else {
        if (sensor2State) {
            if (millis() - lastChangedSensor2 > sensorChangeFilterTime) {
                sensor2State = false;
                ultrasoneCallback();
                lastChangedSensor2 = millis();
            }
        }
    }
}

void button_mode::setup() {
    potentiometer.init();
    potentiometer2.init();
    ultrasoneSensor.init();
    ultrasoneSensor2.init();
    display.init();
    display.backlight();
    display.setCursor(0, 0);
}

void button_mode::loop() {
    updateUltrasoneSensors();
}