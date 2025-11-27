#include "component/door_button.hpp"

#include <HardwareSerial.h>
#include "utils/utils.hpp"

unsigned long lastActivation = 0;

component::DoorButton::DoorButton(const int &pin) : Button(pin, component::callback) {
}

static void component::callback() {
    if (!cooldown(lastActivation)) {
        return;
    }

    utils::selectedClient++;
}
