#include <math.h>
#include <stdio.h>
#include "types.h"

char asChar(Color color)
{
    float gray = (0.299 * (float)color.r + 0.587 * (float)color.g + 0.114 * (float)color.b) / 255.0;
    unsigned int index = roundf(gray * sizeof(GRADIENT) / sizeof(GRADIENT[0]));
    return GRADIENT[index];
}

char *display(Color color)
{
    static char buffer[16];
    snprintf(buffer, sizeof(buffer), "\x1b[38;2;%d;%d;%dm%c\x1b[0m", color.r, color.g, color.b, asChar(color));
    return buffer;
}
