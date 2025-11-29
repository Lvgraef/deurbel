#include "component/bell_button.hpp"

#include "component/door_button.hpp"

component::BellButton::BellButton(const int &pin) : Button(pin, component::callback) {
}

void component::callback() {

}
