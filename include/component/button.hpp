#pragma once
#include <Arduino.h>
#include <esp32-hal.h>

#include "utils/utils.hpp"

namespace component {
    bool debounce(unsigned long& lastActivation, bool& oldState, const bool& currentState);


    static constexpr int debounceTime = 50;

    template <uint8_t PIN> class Button {
    public:
        virtual ~Button() = default;

        constexpr explicit Button(void (*callback)()) : pin(PIN), callback(callback) { }

        void init() const {
            pinMode(pin, INPUT);
            attachInterrupt(digitalPinToInterrupt(pin), callback, CHANGE);
        }
    protected:
        const int pin;
    private:
        void (*callback)();
    };
}
