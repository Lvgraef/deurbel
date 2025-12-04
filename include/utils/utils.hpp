#pragma once
#include <string>
#include <vector>

namespace utils {
    struct Client {
        std::uint8_t clientMacAddress[6];
        std::string name;
    };

    template <typename T> T& getArrayIndexSafe(std::vector<T> array, const std::size_t& index) {
        return array[index % array.size()];
    }
    int16_t getDistanceInput(const float& percentage);
}
