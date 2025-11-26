#pragma once
#include <functional>

namespace component {
    class Button {
    public:
        virtual ~Button() = default;

        explicit Button(const int& pin);

        void init() const;
        void update();

        virtual void callback() = 0;
    private:
        unsigned long lastActivation = 0;
        const int pin;
        bool state = false;
    };
}
