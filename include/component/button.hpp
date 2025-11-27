#pragma once
#include <esp32-hal.h>

namespace component {
    static constexpr int cooldownTime = 200;

    class Button {
    public:
        virtual ~Button() = default;

        explicit Button(const int& pin, void (*callback)());

        void init() const;
    protected:
        const int pin;
    private:
        void (*callback)();
    };

    bool cooldown(unsigned long& lastActivation);
}