#pragma once

#include "button.hpp"

namespace component {
    class BellButton final : public Button {
    public:
        explicit BellButton(const int &pin);
    };

    static void callback();
}
