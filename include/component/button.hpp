#pragma once
#include <Arduino.h>
#include <esp32-hal.h>

namespace component {
    /// Debounces the button input
    /// @param lastActivation The time of the last activation in milliseconds
    /// @param oldState The previous state of the button
    /// @param currentState The current state of the button
    /// @return returns true if the activation was an actual input and false if it was a bounce
    bool debounce(unsigned long& lastActivation, bool& oldState, const bool& currentState);

    /// debounce time in milliseconds
    static constexpr int debounceTime = 100;

    template <uint8_t PIN> class Button {
    public:
        virtual ~Button() = default;

        /// Create a new button
        /// @param callback The callback that should be run on the interrupt (keep this short)
        /// @param pullup If the input should be pulled up or pulled down
        constexpr explicit Button(void (*callback)(), const bool& pullup) : callback(callback), pullup(pullup) { }

        /// Initialise the button
        void init() const {
            // pinMode(PIN, pullup ? INPUT_PULLUP : INPUT_PULLDOWN);
            pinMode(PIN, INPUT);
            // This attaches an ISR which is triggered when the state of the pin changes from high to low or low to high
            attachInterrupt(digitalPinToInterrupt(PIN), callback, CHANGE);
        }
    private:
        void (*callback)();
        const bool pullup;
    };
}
