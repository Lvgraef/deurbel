#include "button_mode/button_main.hpp"

#include <LiquidCrystal_I2C.h>
#include <Adafruit_MCP23X17.h>
#include <memory>

#include "component/bell_button.hpp"
#include "component/door_button.hpp"
#include "component/sync_button.hpp"
#include "component/potentiometer.hpp"
#include "component/ultrasone_sensor.hpp"

#define MAX_CLIENTS 16

// one minute
constexpr unsigned long overrideLimit = 60000;

std::vector<utils::Client> button_mode::clients;
size_t button_mode::selectedClient = 0;

bool volatile button_mode::overridden = false;
unsigned volatile long button_mode::overrideTime = ULONG_MAX;


component::Potentiometer potentiometer(39);

std::unique_ptr<component::UltrasoneSensor> ultrasoneSensors[6] = {
    std::make_unique<component::UltrasoneSensor>(32, 14),
    std::make_unique<component::UltrasoneSensor>(1, 13),
    std::make_unique<component::UltrasoneSensor>(2, 13),
    std::make_unique<component::UltrasoneSensor>(3, 13),
    std::make_unique<component::UltrasoneSensor>(4, 13),
    std::make_unique<component::UltrasoneSensor>(5, 13)

};

bool ultrasoneStates[16] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
unsigned long lastChangedSensors[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

component::DoorButton<34> doorButton;
component::BellButton<36> bellButton;
component::SyncButton<15> syncButton;

LiquidCrystal_I2C display(0x27, 16, 2);

constexpr int sensorChangeFilterTime = 500;

[[noreturn]] void updateUltrasoneSensors(void* params) {
    while (true) {
        // set this too false when the bell is rung!
        if (button_mode::overridden && (millis() - button_mode::overrideTime < overrideLimit)) {
            Serial.println(std::to_string((millis() - button_mode::overrideTime)).c_str());
            continue;
        }
        button_mode::overrideTime = ULONG_MAX;
        button_mode::overridden = false;
        const int16_t distanceInput = utils::getDistanceInput(potentiometer.getValue());
        for (int i = 0; i < button_mode::clients.size(); i++) {
            ultrasoneSensors[i]->update();
            if (i == 0) {
                Serial.println(std::to_string(ultrasoneSensors[i]->getDistance()).c_str());
            }
            if (ultrasoneSensors[i]->getDistance() != 0 && ultrasoneSensors[i]->getDistance() < distanceInput) {
                if (!ultrasoneStates[i]) {
                    if (millis() - lastChangedSensors[i] > sensorChangeFilterTime) {
                        ultrasoneStates[i] = true;
                        button_mode::selectedClient = i;
                        lastChangedSensors[i] = millis();
                        Serial.println("button_mode::selectedClient");
                    }
                }
            } else {
                if (ultrasoneStates[i]) {
                    if (millis() - lastChangedSensors[i] > sensorChangeFilterTime) {
                        ultrasoneStates[i] = false;
                        lastChangedSensors[i] = millis();
                    }
                }
            }
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

[[noreturn]] void updateLCDDisplay(void* params) {
    while (true) {
        const std::string& displayName = utils::getArrayIndexSafe(button_mode::clients, button_mode::selectedClient).name;

        display.clear();
        display.print(displayName.c_str());

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void button_mode::setup() {
    potentiometer.init();
    display.init();
    display.backlight();
    syncButton.init();
    bellButton.init();
    doorButton.init();

    Wire.setClock(100000);

    if (!mcp.begin_I2C(0x20)) {
        Serial.println("GPIO expander init failed");
    }

    // For now add 2 clients:
    clients.push_back({{0xAA, 0x1A, 0x3F, 0xEE, 0x3F, 0xB2}, "Luc"});
    //clients.push_back({{0x06, 0x2E, 0xBD, 0x40, 0xE3, 0xC2}, "Raven"});

    for (int i = 0; i < clients.size(); i++) {
        if (const auto& sensor = ultrasoneSensors[i]) {
            sensor->init();
        }
    }

    xTaskCreate(updateUltrasoneSensors, "ultrasone_sensors", 4096, nullptr, 1, nullptr);
    xTaskCreate(updateLCDDisplay, "lcddisplay", 4096, nullptr, 1, nullptr);

    pinMode(13, INPUT);
    mcp.pinMode(0, OUTPUT);
}

void button_mode::loop() {
    for (int i = 0; i < clients.size(); i++) {
        if (const auto& sensor = ultrasoneSensors[i]) {
            sensor->update();
        }
        delay(5);
    }
}