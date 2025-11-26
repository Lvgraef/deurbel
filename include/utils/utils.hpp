#pragma once
#include <cstdio>

namespace utils {
    template <typename T, std::size_t N> T& getArrayIndexSafe(T (&array)[N], const std::size_t& index);
}