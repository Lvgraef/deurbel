#include "utils/utils.hpp"


int16_t utils::getDistanceInput(const float &percentage)  {
    constexpr int minDistance = 10;
    constexpr int maxDistance = 300;

    return percentage * (maxDistance - minDistance) + minDistance;
}
