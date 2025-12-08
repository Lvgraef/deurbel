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

static component::DoorButton<34> doorButton;
static component::BellButton<36> bellButton;
static component::SyncButton<15> syncButton;

bool updateDisplay = false;

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
            vTaskDelay(20 / portTICK_PERIOD_MS);
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

/// Updates the LCD display so it displays the number of the selected client/peer
void lcdDislayUpdate() {
    if (const auto peer = networking::Server::getSelectedPeer(); peer.has_value()) {
        const std::string displayName = std::to_string(peer.value().number);
        display.clear();
        display.print(displayName.c_str());
    } else {
        display.clear();
        display.print("No peers!");
    }

}


/// Gets called when a sensor starts or stops detecting something.
/// @param state Whether the sensor started or stopped a detection
/// @param index The index of the sensor
void ultrasoneStateChanged(bool state, int index) {
    if (state) {
        networking::Server::sendToPeer(networking::LED_ON);
    } else {
        networking::Server::sendToPeer(networking::LED_OFF);
    }

    lcdDislayUpdate();
}

/// Handles the ultrasone sensor logic
void getUltrasoneDistances() {
    // Skip the ultrasone logic if the door is overridden.
    if (!(button_mode::overridden && (millis() - button_mode::overrideTime < overrideLimit))) {
        button_mode::overrideTime = ULONG_MAX;
        button_mode::overridden = false;

        const int16_t distanceInput = utils::getDistanceInput(potentiometer.getValue());

        for (int i = 0; i < networking::Server::getPeerCount(); i++) {
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

void button_mode::setup() {
    if (!networking::Server::begin()) {
        Serial.println("server begin failed");
    }

    Serial2.begin(9600, SERIAL_8N1, 7, 8);

    potentiometer.init();
    display.init();
    display.backlight();
    syncButton.init();
    bellButton.init();
    doorButton.init();

    Wire.setClock(100000);

    for (int i = 0; i < networking::Server::getPeerCount(); i++) {
        if (const auto& sensor = ultrasoneSensors[i]) {
            sensor->init();
        }
    }

    xTaskCreate(updateUltrasoneSensors, "ultrasone_sensors", 4096, nullptr, 1, nullptr);
}

void button_mode::loop() {
    getUltrasoneDistances();
    syncButton.update();
    bellButton.update();
    if (updateDisplay) {
        updateDisplay = false;
        lcdDislayUpdate();
    }
    delay(10);
}

void button_mode::updateLCDDisplay() {
    updateDisplay = true;
}
