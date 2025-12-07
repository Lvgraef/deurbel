#pragma once

namespace component {
    class Buzzer {
    public:
        /// Create a new buzzer
        /// @param pin The pin the buzzer is attached to.
        explicit Buzzer(const int& pin);

        /// Initialize the buzzer
        void init() const;

        /// Make a short beep sound
        void beep() const;

    private:
        const int pin;
    };
}