#pragma once
#include <cstdint>
#include <cstdio>

namespace utils {
    extern std::uint8_t clientMacAddresses[1][6];
    extern size_t selectedClient;

    template <typename T, std::size_t N> T& getArrayIndexSafe(T (&array)[N], const std::size_t& index);
}