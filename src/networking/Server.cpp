#include "networking/Server.hpp"
#include "utils/utils.hpp"

using namespace networking;

// Broadcast MAC
static uint8_t broadcastAddress[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

std::vector<networking::Client> networking::Server::peers;
size_t networking::Server::selectedPeer = 0;

bool networking::Server::begin() {
    Serial.println("[SERVER] Begin");

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) return false;

    esp_now_register_recv_cb(onDataRecv);
    esp_now_register_send_cb(onDataSent);

    // Add broadcast peer
    esp_now_peer_info_t broadcastPeer{};
    memcpy(broadcastPeer.peer_addr, broadcastAddress, 6);
    broadcastPeer.channel = 0;
    broadcastPeer.encrypt = false;
    esp_now_add_peer(&broadcastPeer);

    return true;
}

void networking::Server::startPairing() {
    uint8_t msg = HELLO;
    esp_now_send(broadcastAddress, &msg, 1);
}

void networking::Server::onDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
    if (len != 1) return;

    uint8_t msgType = data[0];

    if (msgType == HELLO) {
        // New peer discovery
        Serial.println("[PAIR] HELLO received, adding peer...");
        addPeer(mac, msgType);

        // Reply back with HELLO so the sender can add us
        esp_now_send(mac, &msgType, 1);
    }
}

void networking::Server::onDataSent(const uint8_t *mac, esp_now_send_status_t status) {
    Serial.printf("[SEND] To %02X:%02X:%02X:%02X:%02X:%02X Status: %s\n",
                  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
                  status == ESP_NOW_SEND_SUCCESS ? "OK" : "FAIL");
}

bool networking::Server::sendToPeer(const uint8_t msg) {
    if (const auto peer = getSelectedPeer(); peer.has_value()) {
        return esp_now_send(peer.value().clientMacAddress, &msg, 1) == ESP_OK;
    }
    Serial.println("[ERROR] No peer to send to");
    return false;
}

bool networking::Server::sendToPeer(const uint8_t mac[6], uint8_t msg) {
    return esp_now_send(mac, &msg, 1) == ESP_OK;
}

// Add a peer if not already present
void networking::Server::addPeer(const uint8_t *mac, const uint8_t number) {
    for (auto &[clientMacAddress, num]: peers) {
        if (memcmp(clientMacAddress, mac, 6) == 0) return;
    }
    esp_now_peer_info_t info{};
    memcpy(info.peer_addr, mac, 6);
    info.channel = 0;
    info.encrypt = false;
    if (esp_now_add_peer(&info) == ESP_OK) {
        peers.push_back({{
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]
        }, number});
        Serial.printf("[PAIR] Peer added: %02X:%02X:%02X:%02X:%02X:%02X\n",
                      mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }
}

void networking::Server::selectPeer(const int peer) {
    selectedPeer = peer;
}

std::optional<networking::Client> networking::Server::getSelectedPeer() {
    if (peers.size() == 0) {
        return std::nullopt;
    }
    return utils::getArrayIndexSafe(peers, selectedPeer);
}

int networking::Server::getPeerCount() {
    return peers.size();
}

void networking::Server::cyclePeer() {
    selectedPeer++;
}
