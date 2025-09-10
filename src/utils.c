#include "utils.h"

float clamp(const float value, const float min, const float max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}
