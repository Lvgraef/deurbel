#pragma once
#include <esp32-hal.h>

#include "button.hpp"
#include "utils/utils.hpp"
#include "networking/Server.hpp"

namespace component {
    template <uint8_t PIN> class SyncButton : public Button<PIN> {
    public:
        constexpr explicit SyncButton() : Button<PIN>(&SyncButton::isr, false) { }

        /// This should be continuously called to do expensive operations when the button has been pressed.
        void update() {
            if (state) {
                state = false;

                networking::Server::startPairing();
            }
        }

        static inline volatile bool state = false;
    private:
        static inline unsigned long lastActivation = 0;
        static inline bool oldState = false;

        /// This interrupt is called when the pin state changes from high to low or low to high
        static void IRAM_ATTR isr() {
            if (!debounce(lastActivation, oldState, digitalRead(PIN))) {
                return;
            }

            state = true;
        }
    };
}
