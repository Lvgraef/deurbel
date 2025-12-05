#pragma once

#include "button.hpp"
#include "networking/Server.hpp"

namespace component {
    template <uint8_t PIN> class BellButton : public Button<PIN> {
    public:
        constexpr explicit BellButton() : Button<PIN>(&BellButton::isr, false) { }

        /// This should be continuously called to do expensive operations when the button has been pressed.
        void update() {
            if (state) {
                state = false;

                // This sends a message that rings the bell on the selected client.
                networking::Server::sendToPeer(networking::BEEP);
            }
        }
    private:
        static inline unsigned long lastActivation;
        static inline bool oldState;
        static inline volatile bool state = false;

        /// This interrupt is called when the pin state changes from high to low or low to high
        static void IRAM_ATTR isr() {
            if (!debounce(lastActivation, oldState, digitalRead(PIN))) {
                return;
            }

            // Set the state to true.
            // this is used in the update to do expensive operations that shouldn't be done in an ISR.
            state = true;
        }
    };
}
