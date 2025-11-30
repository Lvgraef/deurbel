#pragma once
#include <cstdint>
#include <esp_attr.h>
#include <arduino.h>

namespace component {
    // mm/µs
    static constexpr float soundSpeed = 0.343;

    template <uint8_t ECHO, uint8_t TRIG> class UltrasoneSensor {
    public:
        explicit UltrasoneSensor(const std::function<void()> &callback) : callback(callback) {
        }

        static void init() {
            pinMode(ECHO, INPUT);
            pinMode(TRIG, OUTPUT);

            attachInterrupt(ECHO, isr, RISING);
        }

        void update() const {
            if (updated) {
                updated = false;
                callback();
            }
        }

    private:
        std::function<void()> callback;

        static inline unsigned long triggerTime = 0;
        static inline unsigned long echoTime = 0;
        static volatile inline bool updated = false;

        static void IRAM_ATTR isr() {
            echoTime = micros();
            updated = true;
        }
    };
}
