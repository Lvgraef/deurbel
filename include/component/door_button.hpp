#pragma once
#include <esp32-hal.h>

#include "button.hpp"
#include "utils/utils.hpp"

namespace component {
    template <uint8_t PIN> class DoorButton {
    public:
        constexpr explicit DoorButton() : Button<PIN>(&DoorButton::isr) { }
    private:
        static inline unsigned long lastActivation = 0;
        static inline bool oldState = false;


        static void IRAM_ATTR isr() {
            if (!debounce(lastActivation, oldState, digitalRead(PIN))) {
                return;
            }

            utils::selectedClient++;
        }
    };
}
