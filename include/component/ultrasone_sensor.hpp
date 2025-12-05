#pragma once
#include <arduino.h>

namespace component {
    // mm/µs
    static constexpr float soundSpeed = 0.0343;

    class UltrasoneSensor {
    public:
        /// Creates a new ultrasone sensor object
        /// @param trigPin The pin the trigger is attached to (output)
        /// @param echoPin The pin the echo is attached to (input)
        explicit UltrasoneSensor(const uint8_t& trigPin, const uint8_t& echoPin);

        /// Call this once to initialize the sensor
        void init();
        /// Call this repeatedly to keep updating the distance.
        void update();

        /// Get the distance to object in front of the sensor
        /// @return Returns the distance of the closest object in cm
        [[nodiscard]] uint16_t getDistance() const;


    private:
        const uint8_t echoPin;
        const uint8_t trigPin;
        uint16_t distance = 65535;
    };
}
