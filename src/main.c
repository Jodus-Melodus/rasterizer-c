#include <stdio.h>
#include <stdlib.h>
#include "screen.h"
#include "model.h"

int main()
{
    // ScreenBuffer screen;
    // initScreenBuffer(&screen);
    // Vector2 a = {-10.0, -10.0};
    // Vector2 b = {10.0, -10.0};
    // Vector2 c = {0.0, 10.0};
    // Color color = {255, 255, 255};

    // drawTriangle(&screen, a, b, c, color);

    // const char *result = display(&screen);
    // printf("%s\n", result);

    // freeScreenBuffer(&screen);

    Vector3 point = {0.0, 0.0, -5.0};
    Vector2 *projected = projectCoordinate(point, 10.0);
    if (projected == NULL)
    {
        perror("Failed to project point");
        return -1;
    }

    printf("%f %f\n", projected->x, projected->y);
    return 0;
}