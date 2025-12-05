#include "networking/server.hpp"

using namespace networking;
std::vector<std::array<uint8_t, 6>> server::peers;


bool server::begin() {
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) return false;

    esp_now_register_recv_cb(onDataRecv);
    esp_now_register_send_cb(onDataSent);
    return true;
}

void server::startPairing() {
    uint8_t msg = HELLO;
    esp_now_send(nullptr, &msg, 1);  // nullptr is a default for broadcasting
}

void server::onDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
    if (len == 1 && data[0] == ACK) {
        Serial.println("[PAIR] New peer found, adding...");
        addPeer(mac);
    }
}

void server::onDataSent(const uint8_t *mac, esp_now_send_status_t status) {
    Serial.printf("[SEND] To %02X:%02X:%02X:%02X:%02X:%02X Status: %s\n",
                  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
                  status == ESP_NOW_SEND_SUCCESS ? "OK" : "FAIL");
}

bool server::sendToPeer(int index, uint8_t msg) {
    if (index < 0 || index >= peers.size()) {
        Serial.println("[ERROR] Peer index out of range");
        return false;
    }
    return esp_now_send(peers[index].data(), &msg, 1) == ESP_OK;
}

bool server::sendToPeer(const uint8_t mac[6], uint8_t msg) {
    return esp_now_send(mac, &msg, 1) == ESP_OK;
}


void server::addPeer(const uint8_t *mac) {
    for (auto &p: peers) {
        if (memcmp(p.data(), mac, 6) == 0) return;
    }

    esp_now_peer_info_t info{};
    memcpy(info.peer_addr, mac, 6);
    info.channel = 0;
    info.encrypt = false;

    if (esp_now_add_peer(&info) == ESP_OK) {
        peers.push_back({
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]
        });
        Serial.println("[PAIR] Peer added.");
    }
}