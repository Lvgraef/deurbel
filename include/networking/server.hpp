#pragma once
#include <esp_now.h>
#include <WiFi.h>
#include <vector>
#include <array>
#include "networking/protocol.hpp"

namespace networking {
    class server {
    public:
        /// Initialises the Wi-Fi, esp now and registers the callback functions
        /// @return the status of the initialisation
        static bool begin();

        /// Method to start the pairing sequence
        static void startPairing();

        /// Receive callback function
        /// @param mac_addr The mac address of the sender from which the messages is received
        /// @param data The incoming data
        /// @param len the length of the data
        static void onDataRecv(const uint8_t *mac_addr, const uint8_t *data, int len);

        /// Callback function of ESP-NOW
        /// @param mac_addr The mac of the receiver
        /// @param status The status of the message
        static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

        /// This sends to a peer based on index in the list
        /// @param index The index of the peer in the stored list
        /// @param msg The message to send (the max is the value 250)
        /// @return A boolean if sending the message was successful
        static bool sendToPeer(int index, uint8_t msg);

        /// This sends to a peer based on the mac of the peer
        /// @param mac The mac of the peer which should receive the message
        /// @param msg The message to send (the max is the value 250)
        /// @return A boolean if sending the message was successful
        static bool sendToPeer(const uint8_t mac[6], uint8_t msg);

    private:

        /// Method to add a peer to the list which checks if the peer is already in the list
        /// @param mac The mac to be added to the list
        static void addPeer(const uint8_t *mac);
        static std::vector<std::array<uint8_t, 6>> peers;
    };
}