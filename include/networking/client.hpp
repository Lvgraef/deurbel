#pragma once
#include <esp_now.h>
#include <WiFi.h>
#include "networking/protocol.hpp"

namespace networking {
    class EspNowReceiver {
    public:
        /// Constructor to set the buzzerPin and the syncButtonPin
        /// @param buzzerPin
        /// @param syncButtonPin
        EspNowReceiver(int buzzerPin, int syncButtonPin);

        /// Beginning receiving
        bool begin() const;

    private:
        int buzzerPin;
        int syncButtonPin;

        /// Receive callback function
        /// @param mac The mac address of the sender from which the messages is received
        /// @param incomingData The incoming data
        /// @param len the length of the data
        static void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);
    };
}
