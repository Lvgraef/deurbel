#pragma once

#include "button.hpp"

namespace component {
    class BellButton : public Button {
    public:
        explicit BellButton(const int &pin);

        void callback() override;
    };
}