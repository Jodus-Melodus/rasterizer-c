#include <stdio.h>
#include <stdlib.h>
#include "screen.h"
#include "model.h"

int main()
{
    struct ScreenBuffer screen;
    initScreenBuffer(&screen);
    struct Vector2 a, b, c;
    float a_init[2] = {-10.0, -10.0};
    float b_init[2] = {10.0, -10.0};
    float c_init[2] = {0.0, 10.0};
    initVector(a.data, a_init, 2);
    initVector(b.data, b_init, 2);
    initVector(c.data, c_init, 2);
    struct Color color = {255, 255, 255};

    drawTriangle(&screen, a, b, c, color);

    const char *result = display(&screen);
    printf("%s\n", result);

    freeScreenBuffer(&screen);
    return 0;
}