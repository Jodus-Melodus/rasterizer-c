#ifndef TYPES_H
#define TYPES_H

static const char GRADIENT[10] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};

typedef struct
{
    unsigned char r, g, b;
} Color;

char asChar(Color *color);
const char *display(Color *color);

#endif