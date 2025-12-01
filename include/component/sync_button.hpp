#pragma once
#include <esp32-hal.h>

#include "button.hpp"
#include "utils/utils.hpp"

namespace component {
    template <uint8_t PIN> class SyncButton {
    public:
        constexpr explicit SyncButton() : Button<PIN>(&SyncButton::isr, false) { }

        void update() {
            if (state) {
                state = false;

                // Sync
            }
        }
    private:
        static inline unsigned long lastActivation = 0;
        static inline bool oldState = false;
        static inline volatile bool state = false;


        static void IRAM_ATTR isr() {
            if (!debounce(lastActivation, oldState, digitalRead(PIN))) {
                return;
            }

            state = true;
        }
    };
}
