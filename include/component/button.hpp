#pragma once
#include <Arduino.h>
#include <esp32-hal.h>

namespace component {
    bool debounce(unsigned long& lastActivation, bool& oldState, const bool& currentState);


    static constexpr int debounceTime = 100;

    template <uint8_t PIN> class Button {
    public:
        virtual ~Button() = default;

        constexpr explicit Button(void (*callback)(), const bool& pullup) : callback(callback), pullup(pullup) { }

        void init() const {
            pinMode(PIN, pullup ? INPUT_PULLUP : INPUT_PULLDOWN);
            attachInterrupt(digitalPinToInterrupt(PIN), callback, CHANGE);
        }
    private:
        void (*callback)();
        const bool pullup;
    };
}
