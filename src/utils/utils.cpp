#include "utils/utils.hpp"

template <typename T, std::size_t N> T& utils::getArrayIndexSafe(T (&array)[N], const std::size_t& index) {
    return array[index % N];
}
