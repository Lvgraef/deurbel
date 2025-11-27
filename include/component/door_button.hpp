#pragma once

#include "button.hpp"

namespace component {
    class DoorButton : public Button {
    public:
        explicit DoorButton(const int &pin);

        void loop();
    };

    static void callback();
}