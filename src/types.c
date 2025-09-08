#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

Color initColor(unsigned char r, unsigned char g, unsigned char b)
{
    Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    return color;
}

char asChar(Color color)
{
    float gray = (0.299 * (float)color.r + 0.587 * (float)color.g + 0.114 * (float)color.b) / 255.0;
    unsigned int index = round(gray * sizeof(GRADIENT) / sizeof(GRADIENT[0]));
    return GRADIENT[index];
}

char *displayColor(Color color)
{
    char *output = malloc(32);
    if (!output)
        return NULL;
    char character = asChar(color);
    snprintf(output, 32, "\x1b[38;2;%u;%u;%um%c\x1b[0m", color.r, color.g, color.b, character);
    return output;
}
