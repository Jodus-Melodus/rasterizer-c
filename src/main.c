#include "screen.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    struct ScreenBuffer screen;
    initScreenBuffer(&screen);

    set(&screen, 0, 0, 1);

    const char *output = display(&screen);
    printf("%s", output);
    free((void *)output);
    freeScreenBuffer(&screen);

    return 0;
}