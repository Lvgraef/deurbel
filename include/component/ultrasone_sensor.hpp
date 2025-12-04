#pragma once
#include <arduino.h>
#include <Adafruit_MCP23X17.h>

namespace component {
    // mm/µs
    static constexpr float soundSpeed = 0.0343;

    class UltrasoneSensor {
    public:
        explicit UltrasoneSensor(const uint8_t& trigPin, const uint8_t& echoPin);

        void init();
        void update();

        [[nodiscard]] uint16_t getDistance() const;


    private:
        const uint8_t echoPin;
        const uint8_t trigPin;
        uint16_t distance = 65535;
    };
}
