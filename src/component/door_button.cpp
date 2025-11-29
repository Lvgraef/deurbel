#include "component/door_button.hpp"

#include <HardwareSerial.h>
#include "utils/utils.hpp"

unsigned long lastActivation = 0;
bool oldState = false;
int component::pin = -1;

static void callback() {
    if (!component::debounce(lastActivation, oldState, digitalRead(component::pin))) {
        return;
    }

    utils::selectedClient++;
}

void component::createDoorButton(const int& pin) {
    component::pin = pin;
    const Button button(pin, callback);
    button.init();
}
