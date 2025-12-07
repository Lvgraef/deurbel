#include "buzzer_mode/buzzer_main.hpp"

#include <algorithm>
#include <arduino.h>

uint8_t buzzer_mode::number = 0;

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
    inputNumber();
}

void buzzer_mode::loop() {

}