#pragma once

namespace component {
    class Led {
    public:
        /// Create a new LED
        /// @param pin The pin the LED is attached to.
        explicit Led(const int& pin);

        /// Initialize the LED
        void init() const;

        /// turn the LED on
        void on() const;

        /// turn the LED off
        void off() const;

    private:
        const int pin;
    };
}