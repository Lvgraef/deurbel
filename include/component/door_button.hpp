#pragma once
#include <esp32-hal.h>

#include "button.hpp"
#include "button_mode/button_main.hpp"
#include "networking/Server.hpp"

namespace component {
    template <uint8_t PIN> class DoorButton : public Button<PIN> {
    public:
        constexpr explicit DoorButton() : Button<PIN>(&DoorButton::isr, false) { }
    private:
        static inline unsigned long lastActivation = 0;
        static inline bool oldState = false;

        /// This interrupt is called when the pin state changes from high to low or low to high
        static void IRAM_ATTR isr() {
            if (!debounce(lastActivation, oldState, digitalRead(PIN))) {
                return;
            }

            // Cycle to the next peer
            networking::Server::cyclePeer();

            // Set the override variables to disable the sensors
            button_mode::overrideTime = millis();
            button_mode::overridden = true;
            button_mode::updateLCDDisplay();
        }
    };
}
