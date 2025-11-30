#pragma once

#include "button.hpp"

namespace component {
    template <uint8_t PIN> class BellButton {
    public:
        constexpr explicit BellButton() : Button<PIN>(PIN, &BellButton::isr) { }
    private:
        static inline unsigned long lastActivation;
        static inline bool oldState;


        static void IRAM_ATTR isr() {
            if (!debounce(lastActivation, oldState, digitalRead(PIN))) {
                return;
            }

            // do something
        }
    };
}
