#pragma once
#include <esp_attr.h>
#include <arduino.h>

namespace component {
    // mm/µs
    static constexpr float soundSpeed = 0.0343;

    template <uint8_t ECHO, uint8_t TRIG> class UltrasoneSensor {
    public:
        explicit UltrasoneSensor(const Adafruit_MCP23X17& mcp) : mcp(mcp) {
        }

        void init() {
            pinMode(ECHO, INPUT);
            mcp.pinMode(TRIG, OUTPUT);

            attachInterrupt(ECHO, isr, RISING);
        }

        void update() const {
            triggerTime = micros();
            digitalWrite(TRIG, HIGH);
        }

        [[nodiscard]] uint16_t getDistance() const {
            return distance;
        }


    private:
        Adafruit_MCP23X17 mcp;
        static volatile inline uint16_t distance = 65535;
        static inline unsigned long triggerTime = 0;
        static volatile inline bool updated = false;

        static void IRAM_ATTR isr() {
            const auto time = triggerTime - micros();
            distance = time * soundSpeed;
        }
    };
}
