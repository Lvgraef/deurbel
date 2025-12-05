#pragma once
#include <string>
#include <vector>

namespace utils {
    template <typename T> T& getArrayIndexSafe(std::vector<T> array, const std::size_t& index) {
        return array[index % array.size()];
    }
    int16_t getDistanceInput(const float& percentage);
}
