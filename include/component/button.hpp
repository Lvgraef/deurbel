#pragma once
#include <Arduino.h>
#include <esp32-hal.h>

namespace component {
    bool debounce(unsigned long& lastActivation, bool& oldState, const bool& currentState);


    static constexpr int debounceTime = 50;

    template <uint8_t PIN> class Button {
    public:
        virtual ~Button() = default;

        constexpr explicit Button(void (*callback)()) : callback(callback) { }

        void init() const {
            pinMode(PIN, INPUT);
            attachInterrupt(digitalPinToInterrupt(PIN), callback, CHANGE);
        }
    private:
        void (*callback)();
    };
}
