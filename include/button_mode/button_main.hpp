#pragma once
#include <utils/utils.hpp>

namespace button_mode {
    void setup();
    void loop();

    extern volatile bool overridden;
    extern volatile unsigned long overrideTime;
}