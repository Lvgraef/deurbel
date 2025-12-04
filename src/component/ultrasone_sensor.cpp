#include "component/ultrasone_sensor.hpp"

component::UltrasoneSensor::UltrasoneSensor(const uint8_t &trigPin, const uint8_t &echoPin) : echoPin(echoPin), trigPin(trigPin) {
}

void component::UltrasoneSensor::init() {
    pinMode(echoPin, INPUT);
    pinMode(trigPin, OUTPUT);
}

void component::UltrasoneSensor::update() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    const unsigned long duration = pulseIn(echoPin, HIGH, 30000);
    distance = duration / 2.0 * soundSpeed;
}

[[nodiscard]] uint16_t component::UltrasoneSensor::getDistance() const {
    return distance;
}