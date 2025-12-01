#include "button_mode/button_main.hpp"

#include <LiquidCrystal_I2C.h>
#include <Adafruit_MCP23X17.h>
#include <memory>

#include "component/bell_button.hpp"
#include "component/door_button.hpp"
#include "component/sync_button.hpp"
#include "component/potentiometer.hpp"
#include "component/ultrasone_sensor.hpp"

#define MAX_CLIENTS = 16;

std::vector<utils::Client> button_mode::clients;
size_t button_mode::selectedClient = 0;

Adafruit_MCP23X17 mcp;
component::Potentiometer potentiometer(39);

std::unique_ptr<component::UltrasoneInterface> ultrasoneSensors[16] = {
    std::make_unique<component::UltrasoneSensor<0, 13>>(mcp),
    std::make_unique<component::UltrasoneSensor<1, 13>>(mcp),
    std::make_unique<component::UltrasoneSensor<2, 13>>(mcp),
    std::make_unique<component::UltrasoneSensor<3, 13>>(mcp),
    std::make_unique<component::UltrasoneSensor<4, 13>>(mcp),
    std::make_unique<component::UltrasoneSensor<5, 13>>(mcp),
    std::make_unique<component::UltrasoneSensor<6, 13>>(mcp),
    std::make_unique<component::UltrasoneSensor<7, 13>>(mcp),
    std::make_unique<component::UltrasoneSensor<8, 13>>(mcp),
    std::make_unique<component::UltrasoneSensor<9, 13>>(mcp),
    std::make_unique<component::UltrasoneSensor<10, 13>>(mcp),
    std::make_unique<component::UltrasoneSensor<11, 13>>(mcp),
    std::make_unique<component::UltrasoneSensor<12, 13>>(mcp),
    std::make_unique<component::UltrasoneSensor<13, 13>>(mcp),
    std::make_unique<component::UltrasoneSensor<14, 13>>(mcp),
    std::make_unique<component::UltrasoneSensor<15, 13>>(mcp),

};

bool ultrasoneStates[16] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
unsigned long lastChangedSensors[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
component::DoorButton<4> doorButton();
component::BellButton<36> bellButton();
component::SyncButton<15> syncButton();
LiquidCrystal_I2C display(0x20, 16, 2);

constexpr int sensorChangeFilterTime = 500;
void ultrasoneCallback() {

}

//todo add the manual override button so that it doesnt swtich up until 1 minute after youve pressed it or the bell is rang
void updateUltrasoneSensors() {
    const int16_t distanceInput = utils::getDistanceInput(potentiometer.getValue());
    for (int i = 0; i < button_mode::clients.size(); i++) {
        const auto sensor = std::move(ultrasoneSensors[i]);
        sensor->update();
        if (sensor->getDistance() < distanceInput) {
            if (!ultrasoneStates[i]) {
                if (millis() - lastChangedSensors[i] > sensorChangeFilterTime) {
                    ultrasoneStates[i] = true;
                    button_mode::selectedClient = i;
                    // what is this again?!?!?
                    ultrasoneCallback();
                    lastChangedSensors[i] = millis();
                }
            }
        } else {
            if (ultrasoneStates[i]) {
                if (millis() - lastChangedSensors[i] > sensorChangeFilterTime) {
                    ultrasoneStates[i] = false;
                    ultrasoneCallback();
                    lastChangedSensors[i] = millis();
                }
            }
        }
    }
}

void button_mode::setup() {
    potentiometer.init();
    display.init();
    display.backlight();
    display.setCursor(0, 0);
    mcp.begin_I2C(0x21);
}

void button_mode::loop() {
    updateUltrasoneSensors();
}