#include "button_mode/button_main.hpp"

#include <LiquidCrystal_I2C.h>
#include <memory>

#include "component/bell_button.hpp"
#include "component/door_button.hpp"
#include "component/sync_button.hpp"
#include "component/potentiometer.hpp"
#include "component/ultrasone_sensor.hpp"
#include "networking/Server.hpp"

#define MAX_CLIENTS 16

// todo temporary
uint8_t number = 0;

// 30 seconds
constexpr unsigned long overrideLimit = 30000;

bool volatile button_mode::overridden = false;
unsigned volatile long button_mode::overrideTime = ULONG_MAX;

component::Potentiometer potentiometer(39);

// The setup supports 10 sensors/doors
std::unique_ptr<component::UltrasoneSensor> ultrasoneSensors[10] = {
    std::make_unique<component::UltrasoneSensor>(26, 13),
    std::make_unique<component::UltrasoneSensor>(25, 13),
    std::make_unique<component::UltrasoneSensor>(33, 13),
    std::make_unique<component::UltrasoneSensor>(14, 13),
    std::make_unique<component::UltrasoneSensor>(5, 13),
    std::make_unique<component::UltrasoneSensor>(19, 13),
    std::make_unique<component::UltrasoneSensor>(21, 13),
    std::make_unique<component::UltrasoneSensor>(12, 13),
    std::make_unique<component::UltrasoneSensor>(27, 13),
    std::make_unique<component::UltrasoneSensor>(32, 13),
};

bool ultrasoneStates[10] = {false, false, false, false, false, false, false, false, false, false};
unsigned long lastChangedSensors[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

component::DoorButton<34> doorButton;
component::BellButton<36> bellButton;
component::SyncButton<15> syncButton;

LiquidCrystal_I2C display(0x27, 16, 2);

/// A short cooldown period (100 ms) so that short errors in measurement won't continuously switch doors.
constexpr int sensorChangeFilterTime = 100;

/// Update all the ultrasone sensors
[[noreturn]] void updateUltrasoneSensors(void* params) {
    while (true) {
        for (int i = 0; i < networking::Server::getPeerCount(); i++) {
            if (const auto& sensor = ultrasoneSensors[i]) {
                sensor->update();
            }
        }

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

/// Gets called when a sensor starts or stops detecting something.
/// @param state Whether the sensor started or stopped a detection
/// @param index The index of the sensor
void ultrasoneStateChanged(bool state, int index) {
    // Send a message to turn light on or off
}

/// Handles the ultrasone sensor logic
void getUltrasoneDistances() {
    // Skip the ultrasone logic if the door is overridden.
    if (!(button_mode::overridden && (millis() - button_mode::overrideTime < overrideLimit))) {
        button_mode::overrideTime = ULONG_MAX;
        button_mode::overridden = false;

        const int16_t distanceInput = utils::getDistanceInput(potentiometer.getValue());

        for (int i = 0; i < networking::Server::getPeerCount(); i++) {\
            // Filter out 0 measurements (invalid) and check if the sensor is within the threshold
            if (ultrasoneSensors[i]->getDistance() != 0 && ultrasoneSensors[i]->getDistance() < distanceInput) {
                // only do something on change
                if (!ultrasoneStates[i]) {
                    if (millis() - lastChangedSensors[i] > sensorChangeFilterTime) {
                        ultrasoneStates[i] = true;
                        networking::Server::selectPeer(i);
                        lastChangedSensors[i] = millis();
                        ultrasoneStateChanged(true, i);
                    }
                }
            } else {
                // only do something on change
                if (ultrasoneStates[i]) {
                    if (millis() - lastChangedSensors[i] > sensorChangeFilterTime) {
                        ultrasoneStates[i] = false;
                        lastChangedSensors[i] = millis();
                        ultrasoneStateChanged(false, i);
                    }
                }
            }
        }
    }
}

/// Updates the LCD display so it displays the number of the selected client/peer
[[noreturn]] void updateLCDDisplay(void* params) {
    while (true) {
        const std::string displayName = std::to_string(networking::Server::getSelectedPeer().number);

        display.clear();
        display.print(displayName.c_str());

        vTaskDelay(1000 / portTICK_PERIOD_MS);
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
    number = min(255, result);
}

void button_mode::setup() {
    if (!networking::Server::begin()) {
        Serial.println("server begin failed");
    }

    Serial2.begin(9600, SERIAL_8N1, 7, 8);
    // inputNumber();

    potentiometer.init();
    display.init();
    display.backlight();
    syncButton.init();
    bellButton.init();
    doorButton.init();

    Wire.setClock(100000);


    // Add 2 clients for testing purposes
    uint8_t one[6] = {0xAA, 0x1A, 0x3F, 0xEE, 0x3F, 0xB2};
    uint8_t two[6] = {0x06, 0x2E, 0xBD, 0x40, 0xE3, 0xC2};
    networking::Server::addPeer(one, 118);
    networking::Server::addPeer(two, 116);

    for (int i = 0; i < networking::Server::getPeerCount(); i++) {
        if (const auto& sensor = ultrasoneSensors[i]) {
            sensor->init();
        }
    }

    xTaskCreate(updateUltrasoneSensors, "ultrasone_sensors", 4096, nullptr, 1, nullptr);
    xTaskCreate(updateLCDDisplay, "lcddisplay", 4096, nullptr, 1, nullptr);
}

void button_mode::loop() {
    getUltrasoneDistances();
    syncButton.update();
    bellButton.update();
    delay(10);
}