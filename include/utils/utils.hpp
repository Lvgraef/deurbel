#pragma once
#include <cstdio>
#include <string>

namespace utils {
    struct Client {
        std::uint8_t clientMacAddress[6];
        std::string name;
    };

    extern Client clientMacAddresses[1];
    extern size_t selectedClient;

    template <typename T, std::size_t N> T& getArrayIndexSafe(T (&array)[N], const std::size_t& index);

    int16_t getDistanceInput(const float& percentage);
}
