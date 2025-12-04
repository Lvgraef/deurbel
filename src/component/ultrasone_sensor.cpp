#include "component/ultrasone_sensor.hpp"

component::UltrasoneSensor::UltrasoneSensor(Adafruit_MCP23X17 &mcp, const uint8_t &trigPin, const uint8_t &echoPin) : mcp(mcp), echoPin(echoPin), trigPin(trigPin) {
}

void component::UltrasoneSensor::init() {
    pinMode(echoPin, INPUT);
    mcp.pinMode(trigPin, OUTPUT);
}

void component::UltrasoneSensor::update() {
    mcp.digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    mcp.digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    mcp.digitalWrite(trigPin, LOW);

    const unsigned long duration = pulseIn(echoPin, HIGH, 30000);
    distance = duration / 2.0 * soundSpeed;
    Serial.println(std::to_string(distance).c_str());
}

[[nodiscard]] uint16_t component::UltrasoneSensor::getDistance() const {
    return distance;
}