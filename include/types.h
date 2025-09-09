#ifndef TYPES_H
#define TYPES_H

static const char GRADIENT[10] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};

typedef struct
{
    unsigned char r, g, b;
} Color;

Color initColor(unsigned char r, unsigned char g, unsigned char b);
char asChar(const Color color);
char *displayColor(Color color);

typedef enum
{
    X,
    Y,
    Z
} Axis;

#endif