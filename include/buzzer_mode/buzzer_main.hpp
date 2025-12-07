#pragma once
#include <cstdint>

namespace buzzer_mode {
    /// This is called once when the microcontroller is started.
    void setup();
    /// This is called continuously after the microcontroller has been started.
    void loop();

    extern uint8_t number;
}
