#pragma once

namespace networking {
    /// The different message types
    enum Messages {
        HELLO = 0xA0,
        LED_ON = 0xAE,
        LED_OFF = 0xAF,
        BEEP = 0x01
    };

    /// A data structure containing basic info for the clients
    struct Client {
        std::uint8_t clientMacAddress[6];
        std::uint8_t number;
    };
}
