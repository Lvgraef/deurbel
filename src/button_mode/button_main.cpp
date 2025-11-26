#include "button_mode/button_main.hpp"

#include <WiFi.h>
#include "component/button.hpp"

std::uint8_t clientMacAddresses[1][6] = {{244, 101, 11, 52, 27, 40}};
size_t selectedClient = 0;

void button_mode::setup() {
}

void button_mode::loop() {
}