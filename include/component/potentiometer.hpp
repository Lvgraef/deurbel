#pragma once

namespace component {
    class Potentiometer {
    public:
        explicit Potentiometer(const int& pin);

        void init() const;

        float getValue() const;

    private:
        const int pin;
    };
}