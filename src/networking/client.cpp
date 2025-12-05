#include "networking/client.hpp"

using namespace networking;

static int g_buzzerPin = -1;
static int g_syncButtonPin = -1;

//TODO extract the hardware components from this method
void EspNowReceiver::onDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
    if (len < 1) return;

    const uint8_t cmd = data[0];

    if (cmd == HELLO) {
        if (digitalRead(g_syncButtonPin) == LOW) {
            // Send pairing reply
            uint8_t reply = ACK;
            esp_now_send(mac, &reply, 1);
            Serial.println("[PAIR] Reply sent to sender.");
        }
        return;
    }

    if (cmd == BEEP) {
        Serial.println("[CMD] BEEP!");
        digitalWrite(g_buzzerPin, HIGH);
        delay(200);
        digitalWrite(g_buzzerPin, LOW);
        return;
    }

    Serial.printf("[WARN] Unknown command: 0x%02X\n", cmd);
}

EspNowReceiver::EspNowReceiver(int buzzer, int syncButton)
    : buzzerPin(buzzer), syncButtonPin(syncButton) {}

bool EspNowReceiver::begin() const {
    WiFi.mode(WIFI_STA);

    //TODO reconsider the synButtonPin mode (Change this to pull down)
    pinMode(buzzerPin, OUTPUT);
    pinMode(syncButtonPin, INPUT_PULLUP);
    
    g_buzzerPin = buzzerPin;
    g_syncButtonPin = syncButtonPin;

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW Init Failed");
        return false;
    }

    esp_now_register_recv_cb(onDataRecv);

    Serial.println("Receiver ready.");
    return true;
}
