#include <stdio.h>
#include <stdlib.h>
#include "screen.h"
#include "model.h"

int main()
{
    ScreenBuffer screen;
    initScreenBuffer(&screen);
    Vector2 a = {-10.0, -10.0};
    Vector2 b = {10.0, -10.0};
    Vector2 c = {0.0, 10.0};
    Color color = {255, 255, 255};

    drawTriangle(&screen, a, b, c, color);

    const char *result = display(&screen);
    printf("%s\n", result);

    freeScreenBuffer(&screen);
    return 0;
}