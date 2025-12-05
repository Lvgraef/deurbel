#pragma once
#include <cstdint>

namespace networking {
    enum Messages {
        HELLO = 0xA0,
        ACK = 0xAF,
        BEEP = 0x01
    };

    struct Client {
        std::uint8_t clientMacAddress[6];
        std::uint8_t number;
    };
}
