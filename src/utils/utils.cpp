#include "utils/utils.hpp"

std::uint8_t utils::clientMacAddresses[1][6] = {{244, 101, 11, 52, 27, 40}};
size_t utils::selectedClient = 0;

template <typename T, std::size_t N> T& utils::getArrayIndexSafe(T (&array)[N], const std::size_t& index) {
    return array[index % N];
}
