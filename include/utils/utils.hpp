#pragma once
#include <string>
#include <vector>

namespace utils {
    /// Used to get a specific object from a vector using the index. If the index is larger than the vector size it will wrap around.
    /// @tparam T The type of the array/vector
    /// @param array The vector to get the object from
    /// @param index The index you want to get (this can be bigger than the total number of elements)
    /// @return The object with a specific index from the vector, when the index overflows it will wrap around.
    template <typename T> T& getArrayIndexSafe(std::vector<T> array, const std::size_t& index) {
        return array[index % array.size()];
    }

    /// Gets a distance used for sensor input based on a percentage
    /// @param percentage The percentage of the total available distance
    /// @return The amount of distance
    int16_t getDistanceInput(const float& percentage);
}
