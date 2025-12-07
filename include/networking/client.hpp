#pragma once
#include <esp_now.h>
#include <WiFi.h>
#include "networking/protocol.hpp"

namespace networking {
    class EspNowReceiver {
    public:
        /// Constructor to set the buzzerPin and the syncButtonPin
        EspNowReceiver() = default;

        /// Beginning receiving
        bool begin() const;

    private:
        /// Receive callback function
        /// @param mac The mac address of the sender from which the messages is received
        /// @param incomingData The incoming data
        /// @param len the length of the data
        static void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);
    };
}
