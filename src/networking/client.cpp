#include "networking/client.hpp"

#include "buzzer_mode/buzzer_main.hpp"

using namespace networking;

//TODO extract the hardware components from this method
void EspNowReceiver::onDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
    if (len < 1) return;

    const uint8_t cmd = data[0];

    if (cmd == HELLO) {
        if (buzzer_mode::getSyncState()) {
            // Send pairing reply
            const uint8_t reply = buzzer_mode::number;
            esp_now_send(mac, &reply, 1);
            Serial.println("[PAIR] Reply sent to sender.");
        }
        return;
    }

    if (cmd == BEEP) {
        Serial.println("[CMD] BEEP!");
        buzzer_mode::beep();
        return;
    }

    if (cmd == LED_ON) {
        Serial.println("[LED] ON!");
        buzzer_mode::setLED(true);
        return;
    }

    if (cmd == LED_OFF) {
        Serial.println("[LED] OFF!");
        buzzer_mode::setLED(false);
        return;
    }

    Serial.printf("[WARN] Unknown command: 0x%02X\n", cmd);
}

bool EspNowReceiver::begin() const {
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW Init Failed");
        return false;
    }

    esp_now_register_recv_cb(onDataRecv);

    Serial.println("Receiver ready.");
    return true;
}
