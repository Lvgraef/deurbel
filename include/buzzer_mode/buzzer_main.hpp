#pragma once
#include <cstdint>

namespace buzzer_mode {
    /// This is called once when the microcontroller is started.
    void setup();
    /// This is called continuously after the microcontroller has been started.
    void loop();

    /// Get the state of the sync button
    bool getSyncState();

    /// Play a short beep sound
    void beep();

    /// Set the state of the LED
    void setLED(bool state);

    extern uint8_t number;
}
