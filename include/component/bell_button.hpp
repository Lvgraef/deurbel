#pragma once

#include "button.hpp"
#include "networking/Server.hpp"

namespace component {
    template <uint8_t PIN> class BellButton : public Button<PIN> {
    public:
        constexpr explicit BellButton() : Button<PIN>(&BellButton::isr, false) { }

        void update() {
            if (state) {
                state = false;

                networking::Server::sendToPeer(networking::BEEP);
            }
        }
    private:
        static inline unsigned long lastActivation;
        static inline bool oldState;
        static inline volatile bool state = false;

        static void IRAM_ATTR isr() {
            if (!debounce(lastActivation, oldState, digitalRead(PIN))) {
                return;
            }

            state = false;
        }
    };
}
