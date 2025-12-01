#pragma once
#include <utils/utils.hpp>

namespace button_mode {
    void setup();
    void loop();

    extern std::vector<utils::Client> clients;
    extern size_t selectedClient;
}