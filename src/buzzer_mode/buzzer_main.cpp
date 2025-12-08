#include "buzzer_mode/buzzer_main.hpp"

#include <algorithm>
#include <arduino.h>

#include "component/buzzer.hpp"
#include "component/led.hpp"
#include "component/sync_button.hpp"
#include "networking/client.hpp"

uint8_t buzzer_mode::number = 0;
static component::SyncButton<14> syncButton;
static component::Buzzer buzzer(12);
static component::Led led(13);

bool buzzer_mode::getSyncState() {
    const bool toReturn = syncButton.state;
    syncButton.state = false;
    return toReturn;
}

void buzzer_mode::beep() {
    buzzer.beep();
}

void buzzer_mode::setLED(const bool state) {
    if (state) {
        led.on();
    } else {
        led.off();
    }
}


void inputNumber() {
    int collected = 0;
    int result = 0;
    while (collected < 3) {
        if (Serial2.available()) {
            Serial2.println("available");
            switch(Serial2.read()) {
                case 0xE1 :
                    result = result * 10 + 1;
                    collected++;
                    break;
                case 0xE2 :
                    result = result * 10 + 2;
                    collected++;
                    break;
                case 0xE3 :
                    result = result * 10 + 3;
                    collected++;
                    break;
                case 0xE4 :
                    result = result * 10 + 4;
                    collected++;
                    break;
                case 0xE5 :
                    result = result * 10 + 5;
                    collected++;
                    break;
                case 0xE6 :
                    result = result * 10 + 6;
                    collected++;
                    break;
                case 0xE7 :
                    result = result * 10 + 7;
                    collected++;
                    break;
                case 0xE8 :
                    result = result * 10 + 8;
                    collected++;
                    break;
                case 0xE9 :
                    result = result * 10 + 9;
                    collected++;
                    break;
                case 0xEB :
                    result = result * 10 + 0;
                    collected++;
                    break;
                default:
                    collected++;
                    break;
            }
        }
    }
    buzzer_mode::number = std::min(255, result);
}

void buzzer_mode::setup() {
    Serial2.begin(9600, SERIAL_8N1, 7, 8);
    networking::EspNowReceiver rec{};
    inputNumber();
    if (rec.begin()) {
        Serial.println("RECEIVING started");
    }
    buzzer.init();
    syncButton.init();
    led.init();
}

void buzzer_mode::loop() {

}