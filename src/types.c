#include "types.h"

char asChar(Color color)
{
    float gray = (0.299f * color.r + 0.587f * color.g + 0.114f * color.b) / 255.0f;
    if (gray < 0.0f)
        gray = 0.0f;
    if (gray > 1.0f)
        gray = 1.0f;

    size_t n = sizeof(GRADIENT) / sizeof(GRADIENT[0]);
    size_t index = (size_t)roundf(gray * (float)(n - 1));
    return GRADIENT[index];
}