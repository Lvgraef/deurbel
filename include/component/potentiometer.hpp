#pragma once

namespace component {
    class Potentiometer {
    public:
        /// Create a new potiontiometer
        /// @param pin The pin the potiontiometer is attached to.
        explicit Potentiometer(const int& pin);

        /// Initialize the potentiometer
        void init() const;

        /// Get the value of the potentiometer
        /// @return A float between 0 and 1 representing how far the potientiometer has been turned (0 for all the way off and 1 for all the way on)
        float getValue() const;

    private:
        const int pin;
    };
}