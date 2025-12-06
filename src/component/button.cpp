#include "component/button.hpp"

bool component::debounce(unsigned long &lastActivation, bool &oldState, const bool &currentState) {
    if (!oldState && currentState) {
        oldState = currentState;
        if (millis() - lastActivation >= debounceTime) {
            lastActivation = millis();
            return true;
        }
        return false;
    }
    if (oldState != currentState) {
        oldState = currentState;
    }
    return false;
}
