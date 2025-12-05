#pragma once
#include <utils/utils.hpp>

namespace button_mode {
    /// This is called once when the microcontroller is started.
    void setup();
    /// This is called continuously after the microcontroller has been started.
    void loop();

    /// These are used by the door switching button to disable the sensors.
    extern volatile bool overridden;
    extern volatile unsigned long overrideTime;
}